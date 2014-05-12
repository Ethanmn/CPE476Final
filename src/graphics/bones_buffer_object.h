#ifndef BONES_BUFFER_OBJECT_H_
#define BONES_BUFFER_OBJECT_H_

#include <vector>

#include "graphics/gl_adapters/buffer_object.h"
#include "graphics/gl_adapters/gl_types.h"
#include "graphics/gl_adapters/array_buffer_object.h"
#include "graphics/location_maps.h"

std::vector<ArrayBufferObject> createFromBoneIDAndWeights(
         const std::vector<std::vector<AssimpMesh::BoneIDAndWeight>>& data,
         AttributeLocationMap attributes);

#endif // BONES_BUFFER_OBJECT_H_
