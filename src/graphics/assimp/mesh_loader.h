#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include <assimp/anim.h>

#include <boost/serialization/strong_typedef.hpp>
#include <map>
#include <string>
#include <vector>

#include <glm/gtc/quaternion.hpp>

#include "graphics/assimp/bone.h"

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

struct AssimpMesh {
   typedef std::pair<BoneID, float> BoneIDAndWeight;
   std::vector<float> vertex_array;
   std::vector<float> normal_array;
   std::vector<float> uv_array;
   std::vector<unsigned short> index_array;

   // Array of bones for each vertex.
   std::vector<std::vector<BoneIDAndWeight>> bone_weights_array;

   // Canonical array of Bones. Access with BoneID's.
   // Length is the number of bones in the scene NOT the number of vertices.
   std::vector<Bone> bone_array;

   std::vector<BoneAnimation> bone_animation_array;
};

struct MeshLoader {
   AssimpMesh loadMesh(const std::string& path);

  private:
   std::map<std::string, AssimpMesh> meshes_;
};

#endif // MESH_LOADER_H_
