#ifndef GL_TEXTURE_H_
#define GL_TEXTURE_H_

#include <GL/glew.h>
#include <iostream>

inline GLenum glTexture(size_t texture_id) {
   if (texture_id >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - GL_TEXTURE0) {
      std::cerr << "Invalid texture_id for glTexture: " << texture_id << std::endl;
      exit(EXIT_FAILURE);
   }
   return GL_TEXTURE0 + texture_id;
}

#endif // GL_TEXTURE_H_
