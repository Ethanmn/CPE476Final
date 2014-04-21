#ifndef ARRAY_BUFFER_OBJECT_H_
#define ARRAY_BUFFER_OBJECT_H_

#include <vector>

#include "buffer_object.h"
#include "gl_types.h"

struct ArrayBufferObject {
   template <typename T>
   static ArrayBufferObject create(
         const std::vector<T>& data,
         const GLAttributeLocationMap& attributes,
         size_t num_components);

   GLBufferHandle handle; // Handle for where the Array Buffer is stored.
   // Location of the attribute in the given shader program.
   GLAttributeLocationMap attribute_locations;
   size_t num_components; // E.g. Array of vec3s is 3 components.
};

template <typename T>
ArrayBufferObject ArrayBufferObject::create(
      const std::vector<T>& data,
      const GLAttributeLocationMap& attributes,
      size_t num_components) {
   return ArrayBufferObject{
      createBufferObject(data),
      attributes,
      num_components
   };
}

#endif // ARRAY_BUFFER_OBJECT_H_
