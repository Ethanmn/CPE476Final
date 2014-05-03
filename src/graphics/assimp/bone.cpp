#include "graphics/assimp/bone.h"

#include <iostream>

namespace {
   glm::mat4 fromAiMatrix4x4(const aiMatrix4x4& ai_mat) {
      glm::mat4 glm_mat;
      glm_mat[0][0] = ai_mat.a1;
      glm_mat[1][0] = ai_mat.a2;
      glm_mat[2][0] = ai_mat.a3;
      glm_mat[3][0] = ai_mat.a4;

      glm_mat[0][1] = ai_mat.b1;
      glm_mat[1][1] = ai_mat.b2;
      glm_mat[2][1] = ai_mat.b3;
      glm_mat[3][1] = ai_mat.b4;

      glm_mat[0][2] = ai_mat.c1;
      glm_mat[1][2] = ai_mat.c2;
      glm_mat[2][2] = ai_mat.c3;
      glm_mat[3][2] = ai_mat.c4;

      glm_mat[0][3] = ai_mat.d1;
      glm_mat[1][3] = ai_mat.d2;
      glm_mat[2][3] = ai_mat.d3;
      glm_mat[3][3] = ai_mat.d4;
      return glm_mat;
   }
}

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


Bone::Bone(aiBone* ai_bone, aiNode* ai_node, const aiNodeAnim& channel, BoneID bone_id, BoneID parent_id) :
      transform_(fromAiMatrix4x4(ai_node->mTransformation)),
      inverse_bind_pose_(fromAiMatrix4x4(ai_bone->mOffsetMatrix)),
      bone_animation_(BoneAnimation::fromAiAnimNode(channel)),
      bone_id_(bone_id),
      parent_id_(parent_id)
   {}

//static
std::vector<glm::mat4> Bone::calculateBoneTransformations(const std::vector<Bone>& bones) {
   std::vector<glm::mat4> transformations(bones.size());
   std::vector<boost::optional<glm::mat4>> maybe_transformations(bones.size());
   for (const auto& bone : bones) {
      if (!maybe_transformations[bone.id()])
         calculateBoneTransformation(bones, bone, maybe_transformations);
      transformations[bone.id()] = *maybe_transformations[bone.id()];
   }

   assert(transformations.size() == bones.size());
   assert(maybe_transformations.size() == bones.size());
   return transformations;
}

//static
void Bone::calculateBoneTransformation(
      const std::vector<Bone>& bones,
      const Bone& bone,
      std::vector<boost::optional<glm::mat4>>& transformations) {
   glm::mat4 parent_transformation;
   if (!bone.is_root()) {
      if (!transformations[bone.parent_id()]) {
         calculateBoneTransformation(bones, bones[bone.parent_id()], transformations);
      }
      parent_transformation = *transformations[bone.parent_id()];
   }

   glm::mat4 node_transform(bone.transform());
   //TODO: calculate transform for a given time.
   transformations[bone.id()] = parent_transformation * node_transform * bone.inverse_bind_pose();
}
