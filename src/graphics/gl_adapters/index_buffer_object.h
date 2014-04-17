#ifndef INDEX_BUFFER_OBJECT_H_
#define INDEX_BUFFER_OBJECT_H_

#include <vector>

#include "graphics/gl_adapters/gl_types.h"

struct IndexBufferObject {
   GLBufferHandle handle; // Handle for where the Array Buffer is stored.
   size_t size; // Number of elements in the index_buffer.
};

IndexBufferObject createIndexBufferObject(const std::vector<unsigned short>& data);

#endif // INDEX_BUFFER_OBJECT_H_
