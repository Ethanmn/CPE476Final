#ifndef DEFERRED_FRAME_BUFFER_H_
#define DEFERRED_FRAME_BUFFER_H_
#include "graphics/gl_adapters/gl_types.h"
#include "graphics/texture.h"

enum class GBufferType {
   G_BUFF_POS,
   G_BUFF_DIFF,
   G_BUFF_NORM,
   G_BUFF_TEX,
}; 

struct DeferredFrameBuffer {
   DeferredFrameBuffer(unsigned int width, unsigned int height, 
         TextureSlot texture_slot, FBOType fbo_type);

   GLuint initialize(unsigned int width, unsigned int height, FBOType fbo_type);
   void bind();
   int texture_slot() const { return texture_.texture_slot(); }
   Texture texture() const { return texture_; }

  private:
   GLuint fbo_id_;
   GLuint g_buff_textures[4];
   GLuint g_buff_depth_texture;

   Texture position_texture_;
   Texture normal_texture_;
};

#endif // DEFERRED_FRAME_BUFFER_H_
