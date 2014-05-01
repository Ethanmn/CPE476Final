#ifndef BONE_H_
#define BONE_H_

#include <assimp/scene.h>
#include <glm/glm.hpp>

typedef size_t BoneID;

struct Bone {
   Bone(aiBone* ai_bone, aiNode* ai_node, BoneID bone_id);

  private:
   std::string name_, parent_name_;
   glm::mat4 bind_pose_, inverse_bind_pose_;
   BoneID bone_id_;
};

#endif // BONE_H_
