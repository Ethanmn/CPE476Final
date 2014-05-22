#ifndef FRAME_BUFFER_OBJECT_H_
#define FRAME_BUFFER_OBJECT_H_

#include "graphics/gl_adapters/gl_types.h"
#include "graphics/texture.h"

struct FrameBufferObject {
   FrameBufferObject(unsigned int width, unsigned int height, TextureSlot texture_slot);

   GLuint initialize(unsigned int width, unsigned int height);
   void bind();
   void BindForReading();
   int texture_slot() { return texture_.texture_slot(); }

  private:
   GLuint fbo_id_;
   Texture texture_;
};

#endif // FRAME_BUFFER_OBJECT_H_
