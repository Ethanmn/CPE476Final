#ifndef MESH_H_
#define MESH_H_

#include "graphics/attributes.h"
#include "graphics/gl_adapters/array_buffer_object.h"
#include "graphics/gl_adapters/index_buffer_object.h"
#include "graphics/location_maps.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/material.h"
#include "graphics/texture.h"

struct Shaders;

struct Mesh {
   static Mesh fromAssimpMesh(AttributeLocationMap shaders, const AssimpMesh& mesh);
   IndexBufferObject index_buffer_object;
   std::vector<ArrayBufferObject> attribute_buffer_objects;
   glm::vec3 min, max;

   std::vector<Bone> bone_array;
   Animation animation;
};

#endif // MESH_H_
