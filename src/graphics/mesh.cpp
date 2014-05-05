#include "graphics/mesh.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/shaders.h"
#include "graphics/bones_buffer_object.h"

//static
Mesh Mesh::fromAssimpMesh(AttributeLocationMap locations, const AssimpMesh& mesh) {
   if (locations.count(Attribute::VERTEX) == 0) {
      std::clog << "Warning: unused attribute VERTEX in shaders." << std::endl;
   }
   if (locations.count(Attribute::NORMAL) == 0) {
      std::clog << "Warning: unused attribute NORMAL in shaders." << std::endl;
   }
   std::vector<ArrayBufferObject> attributes({
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
         });
   const auto bones_attributes = createFromBoneIDAndWeights(mesh.bone_weights_array, locations);
   attributes.insert(attributes.end(), bones_attributes.begin(), bones_attributes.end());
   return Mesh({
         IndexBufferObject::create(mesh.index_array),
         attributes,
         mesh.bone_array,
   });
}
