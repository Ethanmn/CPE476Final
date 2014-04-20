#include "graphics/mesh.h"

#include "graphics/assimp/mesh_loader.h"
#include "graphics/shaders.h"

//static
Mesh Mesh::fromAssimpMesh(Shaders& shaders, const AssimpMesh& mesh) {
   return {
      IndexBufferObject::create(mesh.index_array),
      {
         ArrayBufferObject::create(
            mesh.vertex_array,
            shaders.getAttributes(Attribute::VERTEX),
            3),
         /*
         ArrayBufferObject::create(
            mesh.normal_array,
            shaders.getAttributes(Attribute::NORMAL),
            3),
         */
         ArrayBufferObject::create(
            mesh.uv_array,
            shaders.getAttributes(Attribute::TEX_COORD),
            2),
      }
   };
}
