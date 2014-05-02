#include "graphics/assimp/bone.h"

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
BoneAnimation BoneAnimation::fromAiAnimNode(BoneID bone_id, const aiNodeAnim& channel) {
   BoneAnimation anim;
   anim.bone_id = bone_id;
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


Bone::Bone(aiBone* ai_bone, aiNode* ai_node, const aiNodeAnim& channel, BoneID bone_id) :
      name_(ai_node->mName.C_Str()),
      parent_name_(ai_node->mParent->mName.C_Str()),
      bind_pose_(fromAiMatrix4x4(ai_node->mTransformation)),
      inverse_bind_pose_(fromAiMatrix4x4(ai_bone->mOffsetMatrix)),
      bone_animation_(BoneAnimation::fromAiAnimNode(bone_id, channel)),
      bone_id_(bone_id)
   {}
