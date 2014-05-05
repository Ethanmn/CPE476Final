#include "mesh_loader.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <iostream>

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
   
   ret.uv_array = mesh.HasTextureCoords(0) ?
         std::vector<float>(
               (float*)(mesh.mTextureCoords[0]),
               (float*)(mesh.mTextureCoords[0]) + mesh.mNumVertices * kNumAxes)
         : std::vector<float>();

   ret.normal_array = std::vector<float>(
         (float*)(mesh.mNormals),
         (float*)(mesh.mNormals) + mesh.mNumVertices * kNumAxes);

   for (unsigned int i = 0; i < mesh.mNumFaces; ++i) {
      ret.index_array.insert(
            ret.index_array.end(),
            mesh.mFaces[i].mIndices,
            mesh.mFaces[i].mIndices + kNumAxes);
   }

   meshes_[path] = ret;
   return ret;
}
