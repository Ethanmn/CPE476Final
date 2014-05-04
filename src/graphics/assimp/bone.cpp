#include "graphics/assimp/bone.h"

#include "graphics/assimp/ai_utils.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

//static
BoneAnimation BoneAnimation::fromAiAnimNode(const aiNodeAnim& channel) {
   BoneAnimation anim;
   for (size_t i = 0; i < channel.mNumPositionKeys; ++i) {
      anim.position_keys.push_back(Vec3Key(channel.mPositionKeys[i]));
   }
   for (size_t i = 0; i < channel.mNumRotationKeys; ++i) {
      anim.rotation_keys.push_back(QuatKey(channel.mRotationKeys[i]));
   }
   for (size_t i = 0; i < channel.mNumScalingKeys; ++i) {
      anim.scale_keys.push_back(Vec3Key(channel.mScalingKeys[i]));
   }
   anim.pre_state = channel.mPreState;
   anim.post_state = channel.mPostState;
   return anim;
}

glm::mat4 BoneAnimation::translation(double time) const {
   assert(position_keys.size() > 1);
   for (size_t i = 0; i < position_keys.size() - 1; ++i) {
      if (time < position_keys[i+1].time) {
         const auto& pos1 = position_keys[i];
         const auto& pos2 = position_keys[i + 1];
         const auto delta_time = pos2.time - pos1.time;
         const auto interp_factor = (time - pos1.time) / delta_time;
         const auto interpolated = glm::mix(pos1.value, pos2.value, interp_factor);
         assert(interp_factor >= 0.0f && interp_factor <= 1.0f);
         return glm::translate(
               glm::mat4(),
               interpolated);
      }
   }
   assert(false && "NOT REACHED");
   return glm::mat4();
}

glm::mat4 BoneAnimation::rotation(double time) const {
   assert(rotation_keys.size() > 1);
   for (size_t i = 0; i < rotation_keys.size() - 1; ++i) {
      if (time < rotation_keys[i+1].time) {
         const auto& rot1 = rotation_keys[i];
         const auto& rot2 = rotation_keys[i + 1];
         const auto delta_time = rot2.time - rot1.time;
         const float interp_factor = (time - rot1.time) / delta_time;
         assert(interp_factor >= 0.0f && interp_factor <= 1.0f);
         return glm::mat4_cast(
               glm::lerp(rot1.value, rot2.value, interp_factor));
      }
   }
   assert(false && "NOT REACHED");
   return glm::mat4();
}

glm::mat4 BoneAnimation::scale(double time) const {
   assert(scale_keys.size() > 1);
   for (size_t i = 0; i < scale_keys.size() - 1; ++i) {
      if (time < scale_keys[i+1].time) {
         const auto& scale1 = scale_keys[i];
         const auto& scale2 = scale_keys[i + 1];
         const auto delta_time = scale2.time - scale1.time;
         const auto interp_factor = (time - scale1.time) / delta_time;
         assert(interp_factor >= 0.0f && interp_factor <= 1.0f);
         return glm::scale(
               glm::mat4(),
               glm::mix(scale1.value, scale2.value, interp_factor));
      }
   }
   assert(false && "NOT REACHED");
   return glm::mat4();
}


Bone::Bone(aiBone* ai_bone,
         aiNode* ai_node,
         aiNodeAnim* channel,
         const glm::mat4& global_inverse_transform,
         BoneID bone_id, BoneID parent_id) :
   name_(ai_node->mName.C_Str()),
   transform_(fromAiMatrix4x4(ai_node->mTransformation)),
   global_inverse_transform_(global_inverse_transform),
   inverse_bind_pose_(fromAiMatrix4x4(ai_bone->mOffsetMatrix)),
   bone_animation_(channel ? boost::make_optional(BoneAnimation::fromAiAnimNode(*channel)) : boost::none),
   bone_id_(bone_id),
   parent_id_(parent_id)
{}

//static
std::vector<glm::mat4> Bone::calculateBoneTransformations(
      const std::vector<Bone>& bones,
      double time) {
   std::vector<glm::mat4> transformations(bones.size());
   std::vector<boost::optional<glm::mat4>> maybe_transformations(bones.size());
   for (const auto& bone : bones) {
      if (!maybe_transformations[bone.id()])
         calculateBoneTransformation(bones, time, bone, maybe_transformations);
      transformations[bone.id()] = *maybe_transformations[bone.id()] * bone.inverse_bind_pose();
   }

   assert(transformations.size() == bones.size());
   assert(maybe_transformations.size() == bones.size());
   return transformations;
}

//static
void Bone::calculateBoneTransformation(
      const std::vector<Bone>& bones,
      double time,
      const Bone& bone,
      std::vector<boost::optional<glm::mat4>>& transformations) {
   glm::mat4 parent_transformation;
   if (!bone.is_root()) {
      if (!transformations[bone.parent_id()]) {
         calculateBoneTransformation(bones, time, bones[bone.parent_id()], transformations);
      }
      parent_transformation = *transformations[bone.parent_id()];
   }

   // This is the default pose (no animation)
   glm::mat4 node_transform(bone.transform());
   if (bone.bone_animation_ && bone.name_ == "joint1") {
      const auto translate(bone.bone_animation_->translation(time));
      const auto rotate(bone.bone_animation_->rotation(time));
      const auto scale(bone.bone_animation_->scale(time));
      node_transform = translate * rotate * scale;
   }
   transformations[bone.id()] = parent_transformation * node_transform;
}
