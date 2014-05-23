#ifndef FRAME_BUFFER_OBJECT_H_
#define FRAME_BUFFER_OBJECT_H_

#include "graphics/gl_adapters/gl_types.h"
#include "graphics/texture.h"

struct FrameBufferObject {
   FrameBufferObject(unsigned int width, unsigned int height);

   GLuint initialize(unsigned int width, unsigned int height);
   void bind();
   void BindForReading();
   int texture_id() { return texture_.textureID(); }
   //Texture texture() { return texture_; }

  private:
   GLuint fbo_id_;
   Texture texture_;
};

#endif // FRAME_BUFFER_OBJECT_H_
