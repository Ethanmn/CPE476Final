#ifndef BONE_H_
#define BONE_H_

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

typedef size_t BoneID;

struct Vec3Key {
   Vec3Key(const aiVectorKey& key) :
      time(key.mTime),
      value(glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z))
   {}
   double time;
   glm::vec3 value;
};

struct QuatKey {
   QuatKey(const aiQuatKey& key) :
      time(key.mTime),
      value(glm::quat(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w))
   {}
   double time;
   glm::quat value;
};

struct BoneAnimation {
   static BoneAnimation fromAiAnimNode(BoneID bone_id, const aiNodeAnim& channel);
   BoneID bone_id;

   std::vector<Vec3Key> position_keys;
   std::vector<QuatKey> rotation_keys;
   std::vector<Vec3Key> scale_keys;

   aiAnimBehaviour pre_state, post_state;
};

struct Bone {
   Bone(aiBone* ai_bone, aiNode* ai_node, const aiNodeAnim& channel, BoneID bone_id);

   std::string name() const { return name_; }

  private:
   std::string name_, parent_name_;
   glm::mat4 bind_pose_, inverse_bind_pose_;
   BoneAnimation bone_animation_;
   BoneID bone_id_;
};

#endif // BONE_H_
