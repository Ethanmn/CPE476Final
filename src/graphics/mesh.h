#ifndef MESH_H_
#define MESH_H_

#include "graphics/attributes.h"
#include "graphics/gl_adapters/array_buffer_object.h"
#include "graphics/gl_adapters/index_buffer_object.h"
#include "graphics/location_maps.h"
#include "graphics/material.h"
#include "graphics/texture.h"

struct AssimpMesh;
struct Shaders;

struct Mesh {
   static Mesh fromAssimpMesh(AttributeLocationMap shaders, const AssimpMesh& mesh);
   IndexBufferObject index_buffer_object;
   std::vector<ArrayBufferObject> attribute_buffer_objects;
   Material material;
   Texture texture;
};

#endif // MESH_H_
