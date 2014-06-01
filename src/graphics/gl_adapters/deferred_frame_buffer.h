#ifndef DEFERRED_FRAME_BUFFER_H_
#define DEFERRED_FRAME_BUFFER_H_
#include "graphics/gl_adapters/gl_types.h"
#include "graphics/texture.h"

enum class GBufferType {
   G_BUFF_POS,
   G_BUFF_DIFFUSE,
   G_BUFF_NORM,
}; 

struct DeferredFrameBuffer {
   DeferredFrameBuffer(unsigned int width, unsigned int height); 

   void Initialize(unsigned int width, unsigned int height);
   void bind();
   void SetBufferToRead(GBufferType buffType);
   //int texture_slot() const { return texture_.texture_slot(); }
   //Texture texture() const { return texture_; }

  private:
   GLuint fbo_id_;
   GLuint g_buff_textures[3];
   GLuint g_buff_depth_texture;

   //Texture position_texture_;
   //Texture normal_texture_;
};

#endif // DEFERRED_FRAME_BUFFER_H_
