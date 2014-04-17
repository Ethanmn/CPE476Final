#ifndef BUFFER_OBJECT_H_
#define BUFFER_OBJECT_H_

#include <vector>

#include "gl_types.h"

template <typename T>
GLBufferHandle createBufferObject(const std::vector<T>& data) {
   GLuint vbo;
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
   return GLBufferHandle(vbo);
}

#endif // BUFFER_OBJECT_H_
