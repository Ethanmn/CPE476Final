#include "bones_buffer_object.h"

#include <iostream>

#include "graphics/attributes.h"

std::vector<ArrayBufferObject> createFromBoneIDAndWeights(
         const std::vector<std::vector<AssimpMesh::BoneIDAndWeight>>& data,
         AttributeLocationMap locations) {
   // -1 is a sentinel value. Means: does not have an index.
   std::vector<std::vector<int>> bone_ids(kMaxBonesPerVertex, std::vector<int>(data.size(), -1));
   std::vector<std::vector<float>> bone_weights(kMaxBonesPerVertex, std::vector<float>(data.size()));
   for (size_t vi = 0; vi < data.size(); ++vi) {
      auto& vertex = data[vi];
      assert(vertex.size() <= kMaxBonesPerVertex);
      for (size_t bi = 0; bi < vertex.size(); ++bi) {
         auto& bone = vertex[bi];
         bone_ids[bi][vi] = bone.first;
         bone_weights[bi][vi] = bone.second;
      }
   }
   std::vector<ArrayBufferObject> ret;
   for (size_t i = 0; i < kMaxBonesPerVertex; ++i) {
      ret.push_back(ArrayBufferObject::create(
               bone_ids[i],
               locations[static_cast<Attribute>(static_cast<size_t>(Attribute::BONE_ID0) + i)],
               1));
      ret.push_back(ArrayBufferObject::create(
               bone_weights[i],
               locations[static_cast<Attribute>(static_cast<size_t>(Attribute::BONE_WEIGHT0) + i)],
               1));
   }
   return ret;
}

