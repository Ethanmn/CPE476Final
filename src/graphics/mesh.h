#ifndef MESH_H_
#define MESH_H_

#include "graphics/attributes.h"
#include "graphics/gl_adapters/array_buffer_object.h"
#include "graphics/gl_adapters/index_buffer_object.h"

struct AssimpMesh;
struct Shaders;

struct Mesh {
   static Mesh fromAssimpMesh(Shaders& shaders, const AssimpMesh& mesh);
   IndexBufferObject index_buffer_object;
   std::vector<ArrayBufferObject> attribute_buffer_objects;
};

#endif // MESH_H_
