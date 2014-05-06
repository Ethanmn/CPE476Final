#ifndef BONE_H_
#define BONE_H_

#include <assimp/scene.h>
#include <boost/optional.hpp>
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
      value(glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z))
   {}
   double time;
   glm::quat value;
};

struct BoneAnimation {
   static BoneAnimation fromAiAnimNode(const aiNodeAnim& channel);

   glm::mat4 translation(double time) const;
   glm::mat4 rotation(double time) const;
   glm::mat4 scale(double time) const;

   std::vector<Vec3Key> position_keys;
   std::vector<QuatKey> rotation_keys;
   std::vector<Vec3Key> scale_keys;

   aiAnimBehaviour pre_state, post_state;
};

struct Bone {
   Bone(aiBone* ai_bone,
         aiNode* ai_node,
         aiNodeAnim* channel,
         BoneID bone_id, BoneID parent_id);

   static std::vector<glm::mat4> calculateBoneTransformations(
         const std::vector<Bone>& bones,
         double time
         );

   bool is_root() const { return parent_id_ == bone_id_; }
   BoneID id() const { return bone_id_; }
   BoneID parent_id() const { return parent_id_; }
   glm::mat4 transform() const { return transform_; }
   glm::mat4 inverse_bind_pose() const { return inverse_bind_pose_; }

  private:
   static void calculateBoneTransformation(
         const std::vector<Bone>& bones,
         double time,
         const Bone& bone,
         std::vector<boost::optional<glm::mat4>>& transformations);

   std::string name_;
   // Transformation of the bone in world space from the root of the scene.
   glm::mat4 transform_;
   // Transform from mesh space into bone space.
   glm::mat4 inverse_bind_pose_;
   // TODO: support multiple animations.
   boost::optional<BoneAnimation> bone_animation_;
   BoneID bone_id_, parent_id_;
};

#endif // BONE_H_
