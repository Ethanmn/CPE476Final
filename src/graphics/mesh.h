#ifndef MESH_H_
#define MESH_H_

#include "graphics/attributes.h"
#include "graphics/gl_adapters/array_buffer_object.h"
#include "graphics/gl_adapters/index_buffer_object.h"
#include "graphics/location_maps.h"
#include "graphics/assimp/mesh_loader.h"

struct Shaders;

struct Mesh {
   static Mesh fromAssimpMesh(AttributeLocationMap shaders, const AssimpMesh& mesh);
   IndexBufferObject index_buffer_object;
   std::vector<ArrayBufferObject> attribute_buffer_objects;

   std::vector<Bone> bone_array;
   glm::mat4 global_inverse_transform;
};

#endif // MESH_H_
