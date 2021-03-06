#ifndef FRAME_BUFFER_OBJECT_H_
#define FRAME_BUFFER_OBJECT_H_

#include "graphics/gl_adapters/gl_types.h"
#include "graphics/texture.h"

enum class FBOType {
   DEPTH,
   COLOR_WITH_DEPTH,
};

struct FrameBufferObject {
   FrameBufferObject(unsigned int width, unsigned int height, TextureSlot texture_slot, FBOType fbo_type);

   GLuint initialize(unsigned int width, unsigned int height, FBOType fbo_type);
   void bind() const;
   int texture_slot() const { return texture_.texture_slot(); }
   Texture texture() const { return texture_; }

  private:
   GLuint fbo_id_;
   Texture texture_;
};

#endif // FRAME_BUFFER_OBJECT_H_
