#include "mesh_loader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>
#include <iostream>
#include <set>
#include <memory>

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
      ret.bone_weights_array.resize(mesh.mNumVertices);
      std::map<std::string, BoneID> bone_name_id_map;
      for (size_t i = 0; i < mesh.mNumBones; ++i) {
         auto& bone = mesh.mBones[i];
         aiNode* node = findAiNodeForBone(bone, scene->mRootNode);
         assert(node != nullptr);

         const auto bone_id = i;
         bone_name_id_map[node->mName.C_Str()] = bone_id;
         ret.bone_array.push_back(Bone(bone, node, bone_id));
         for (size_t i = 0; i < bone->mNumWeights; ++i) {
            auto& weight = bone->mWeights[i];
            ret.bone_weights_array[weight.mVertexId].push_back({bone_id, weight.mWeight});
         }
      }
      assert(ret.bone_array.size() == mesh.mNumBones);
      assert(ret.bone_weights_array.size() == mesh.mNumVertices);

      // Get the animations from the scene, but only if we have bones.
      for (size_t i = 0; i < scene->mNumAnimations; ++i) {
         auto& animation = *scene->mAnimations[i];
         for (size_t i = 0; i < animation.mNumChannels; ++i) {
            auto& channel = *animation.mChannels[i];
            ret.bone_animation_array.push_back(BoneAnimation::fromAiAnimNode(
                     bone_name_id_map[channel.mNodeName.C_Str()], channel));
         }
      }
   }

   meshes_[path] = ret;
   return ret;
}
