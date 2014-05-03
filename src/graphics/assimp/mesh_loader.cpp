#include "mesh_loader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>
#include <iostream>
#include <set>
#include <memory>

#include "ai_utils.h"

namespace {
   aiNode* findAiNodeForBone(aiBone* bone, aiNode* root) {
      if (bone->mName == root->mName) {
         return root;
      }

      for (size_t i = 0; i < root->mNumChildren; ++i) {
         aiNode* node = findAiNodeForBone(bone, root->mChildren[i]);
         if (node != nullptr) {
            return node;
         }
      }
      return nullptr;
   }

   struct AssimpBone {
      AssimpBone(aiBone* ai_bone, aiNode* ai_node, const aiNodeAnim& channel, BoneID bone_id) :
         ai_bone(ai_bone),
         ai_node(ai_node),
         channel(channel),
         bone_id(bone_id) {}

      aiBone* ai_bone;
      aiNode* ai_node;
      const aiNodeAnim& channel;
      BoneID bone_id;
   };

   BoneID findParentBoneID(const std::vector<AssimpBone>& bones, const AssimpBone& child_bone) {
      for (auto& bone : bones) {
         if (bone.ai_node->mName == child_bone.ai_node->mParent->mName) {
            return bone.bone_id;
         }
      }
      return child_bone.bone_id;
   }
}

AssimpMesh MeshLoader::loadMesh(const std::string& path) {
   if (meshes_.count(path) != 0) {
      return meshes_[path];
   }

   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(path,
         aiProcess_CalcTangentSpace       |
         aiProcess_Triangulate            |
         aiProcess_JoinIdenticalVertices  |
         aiProcess_SortByPType);
   if (!scene) {
      std::cerr << "Failed to load: " << path << std::endl;
      exit(EXIT_FAILURE);
   }

   const size_t kNumAxes = 3;
   std::clog << "Loaded " << scene->mNumMeshes << " meshes from " << path << std::endl;
   aiMesh& mesh = *scene->mMeshes[0];
   AssimpMesh ret;
   ret.vertex_array = std::vector<float>(
         (float*)(mesh.mVertices),
         (float*)(mesh.mVertices) + mesh.mNumVertices * kNumAxes);

   ret.uv_array = std::vector<float>(
         (float*)(mesh.mTextureCoords),
         (float*)(mesh.mTextureCoords) + mesh.mNumVertices * kNumAxes);

   ret.normal_array = std::vector<float>(
         (float*)(mesh.mNormals),
         (float*)(mesh.mNormals) + mesh.mNumVertices * kNumAxes);

   for (unsigned int i = 0; i < mesh.mNumFaces; ++i) {
      ret.index_array.insert(
            ret.index_array.end(),
            mesh.mFaces[i].mIndices,
            mesh.mFaces[i].mIndices + kNumAxes);
   }

   // Get the bones/skinning.
   if (mesh.mNumBones > 0) {
      // Get the animations from the scene, but only if we have bones.
      std::map<std::string, aiNodeAnim*> channel_map;
      for (size_t i = 0; i < scene->mNumAnimations; ++i) {
         auto& animation = *scene->mAnimations[i];
         for (size_t i = 0; i < animation.mNumChannels; ++i) {
            auto& channel = animation.mChannels[i];
            channel_map[channel->mNodeName.C_Str()] = channel;
         }
      }

      ret.bone_weights_array.resize(mesh.mNumVertices);
      std::vector<AssimpBone> assimp_bones;
      for (size_t i = 0; i < mesh.mNumBones; ++i) {
         auto& bone = mesh.mBones[i];
         aiNode* node = findAiNodeForBone(bone, scene->mRootNode);
         assert(node != nullptr);

         const auto bone_id = i;
         assimp_bones.push_back(AssimpBone(bone, node, *channel_map[node->mName.C_Str()], bone_id));
         for (size_t i = 0; i < bone->mNumWeights; ++i) {
            auto& weight = bone->mWeights[i];
            ret.bone_weights_array[weight.mVertexId].push_back({bone_id, weight.mWeight});
         }
      }
      assert(assimp_bones.size() == mesh.mNumBones);
      glm::mat4 global_inverse_transform(fromAiMatrix4x4(scene->mRootNode->mTransformation.Inverse()));
      for (const auto& assimp_bone : assimp_bones) {
         ret.bone_array.push_back(Bone(
                  assimp_bone.ai_bone,
                  assimp_bone.ai_node,
                  assimp_bone.channel,
                  global_inverse_transform,
                  assimp_bone.bone_id,
                  findParentBoneID(assimp_bones, assimp_bone)));
      }

      assert(ret.bone_array.size() == mesh.mNumBones);
      assert(ret.bone_weights_array.size() == mesh.mNumVertices);
   }

   meshes_[path] = ret;
   return ret;
}
