#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include <map>
#include <string>
#include <vector>

struct AssimpMesh {
   std::vector<float> vertex_array;
   std::vector<float> normal_array;
   std::vector<float> uv_array;
   std::vector<unsigned short> index_array;
};

struct MeshLoader {
   AssimpMesh loadMesh(const std::string& path);

  private:
   std::map<std::string, AssimpMesh> meshes_;
};

#endif // MESH_LOADER_H_
