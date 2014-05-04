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
   return glm::translate(glm::mat4(), position_keys.front().value);
   for (size_t i = position_keys.size(); i != 0; --i) {
      if (position_keys[i].time < time) {
         return glm::translate(glm::mat4(), position_keys[i].value);
      }
   }
   return glm::mat4();
}

glm::mat4 BoneAnimation::rotation(double time) const {
   return glm::mat4_cast(rotation_keys.front().value);
   for (size_t i = rotation_keys.size(); i != 0; --i) {
      if (rotation_keys[i].time < time) {
         return glm::mat4_cast(rotation_keys[i].value);
      }
   }
   return glm::mat4();
}

glm::mat4 BoneAnimation::scale(double time) const {
   return glm::scale(glm::mat4(), scale_keys.front().value);
   for (size_t i = scale_keys.size(); i != 0; --i) {
      if (scale_keys[i].time < time) {
         return glm::scale(glm::mat4(), scale_keys[i].value);
      }
   }
   return glm::mat4();
}


Bone::Bone(aiBone* ai_bone,
         aiNode* ai_node,
         const aiNodeAnim& channel,
         const glm::mat4& global_inverse_transform,
         BoneID bone_id, BoneID parent_id) :
      transform_(fromAiMatrix4x4(ai_node->mTransformation)),
      global_inverse_transform_(global_inverse_transform),
      inverse_bind_pose_(fromAiMatrix4x4(ai_bone->mOffsetMatrix)),
      bone_animation_(BoneAnimation::fromAiAnimNode(channel)),
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
   //glm::mat4 node_transform(bone.transform());

   const auto translate(bone.bone_animation_.translation(time));
   const auto rotate(bone.bone_animation_.rotation(time));
   const auto scale(bone.bone_animation_.scale(time));
   const auto node_transform = translate;
   transformations[bone.id()] = parent_transformation * node_transform;
}
