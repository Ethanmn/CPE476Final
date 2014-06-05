#ifndef DEFERRED_FRAME_BUFFER_H_
#define DEFERRED_FRAME_BUFFER_H_
#include "graphics/gl_adapters/gl_types.h"
#include "graphics/texture.h"
#include <vector>

enum class GBufferType {
   G_BUFF_DIFFUSE,
   G_BUFF_POS,
   G_BUFF_NORM,
}; 

struct DeferredFrameBuffer {
   DeferredFrameBuffer(unsigned int width, unsigned int height); 

   void Initialize(unsigned int width, unsigned int height);
   void bind() const;
   void SetBufferToRead(GBufferType buffType);


   int diffuse_texture_slot() const;
   Texture diffuse_texture() const;
   int position_texture_slot() const;
   Texture position_texture() const;
   int normal_texture_slot() const;
   Texture normal_texture() const;


  private:
   GLuint fbo_id_;
   GLuint g_buff_textures_[3];
   GLuint g_buff_depth_texture_;

   std::vector<Texture> deferred_textures_;
};

#endif // DEFERRED_FRAME_BUFFER_H_
