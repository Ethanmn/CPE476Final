#ifndef ARRAY_BUFFER_OBJECT_H_
#define ARRAY_BUFFER_OBJECT_H_

#include "gl_types.h"

struct ArrayBufferObject {
   GLBufferHandle handle; // Handle for where the Array Buffer is stored.
   // Location of the attribute in the given shader program.
   GLAttributeLocationMap attribute_locations;
   size_t num_components; // E.g. Array of vec3s is 3 components.
};

#endif // ARRAY_BUFFER_OBJECT_H_
