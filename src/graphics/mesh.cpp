#include "graphics/mesh.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/shaders.h"

//static
Mesh Mesh::fromAssimpMesh(AttributeLocationMap locations, const AssimpMesh& mesh) {
   if (locations.count(Attribute::VERTEX) == 0) {
      std::clog << "Warning: unused attribute VERTEX in shaders." << std::endl;
   }
   if (locations.count(Attribute::NORMAL) == 0) {
      std::clog << "Warning: unused attribute NORMAL in shaders." << std::endl;
   }
   return {
      IndexBufferObject::create(mesh.index_array),
      {
         ArrayBufferObject::create(
            mesh.vertex_array,
            locations[Attribute::VERTEX],
            3),
         ArrayBufferObject::create(
            mesh.normal_array,
            locations[Attribute::NORMAL],
            3),
         ArrayBufferObject::create(
            mesh.uv_array,
            locations[Attribute::TEX_COORD],
            2),
         
      }
   };
}
