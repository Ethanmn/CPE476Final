#ifndef BUFFER_OBJECT_H_
#define BUFFER_OBJECT_H_

#include "graphics/gl_adapters/array_buffer_object.h"
#include "graphics/gl_adapters/index_buffer_object.h"

template <typename T>
GLBufferHandle createBufferObject(const std::vector<T>& data) {
   GLuint vbo;
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
   return GLBufferHandle(vbo);
}

template <typename T>
ArrayBufferObject createArrayBufferObject(
      const std::vector<T>& data,
      const std::map<GLShaderHandle, GLAttributeLocation>& attributes,
      size_t num_components) {
   return ArrayBufferObject{
      createBufferObject(data),
      attributes,
      num_components};
}

#endif // BUFFER_OBJECT_H_
