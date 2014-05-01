#include "mesh_loader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>
#include <iostream>
#include <set>
#include <memory>

namespace {

   aiNode* findAiNode(aiBone* bone, aiNode* root) {
      if (bone->mName == root->mName) {
         return root;
      }

      for (size_t i = 0; i < root->mNumChildren; ++i) {
         aiNode* node = findAiNode(bone, root->mChildren[i]);
         if (node != nullptr) {
            return node;
         }
      }
      return nullptr;
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

   // Get the bones.
   if (mesh.mNumBones > 0) {
      ret.bone_weights_array.resize(mesh.mNumVertices);
      for (size_t i = 0; i < mesh.mNumBones; ++i) {
         auto& bone = mesh.mBones[i];
         aiNode* node = findAiNode(bone, scene->mRootNode);
         assert(node != nullptr);

         const auto bone_id = i;
         ret.bone_array.push_back(Bone(bone, node, bone_id));

         for (size_t i = 0; i < bone->mNumWeights; ++i) {
            auto& weight = bone->mWeights[i];
            ret.bone_weights_array[weight.mVertexId].push_back({bone_id, weight.mWeight});
         }
      }
      assert(ret.bone_array.size() == mesh.mNumBones);
      assert(ret.bone_weights_array.size() == mesh.mNumVertices);
   }

   meshes_[path] = ret;
   return ret;
}
