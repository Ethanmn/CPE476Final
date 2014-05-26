#include "deferred_frame_buffer.h"

#include <stdio.h>
#include <assert.h>
#define NUM_DEF_TEX 4

void GenDeferredTexture(GLuint *texture_id, unsigned int width, unsigned int height) {
   glGenTextures(1, texture_id);
   glBindTexture(GL_TEXTURE_2D, *texture_id); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 
         0, GL_RGB, GL_FLOAT, NULL);
   
   //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void GenDepthDeferredTexture(GLuint *texture_id, unsigned int width, unsigned int height) {
   glGenTextures(1, texture_id);
   glBindTexture(GL_TEXTURE_2D, *texture_id);
   
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 
         0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   
   //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
         GL_TEXTURE_2D, *texture_id, 0);
}


GLuint DeferredFrameBuffer::initialize(unsigned int width, unsigned int height) {
   glGenFramebuffers(1, &fbo_id_);
   glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);

   for(int i = 0; i < NUM_DEF_TEX; i++) {
      GenDeferredTexture(&g_buff_textures[i], width, height);  
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
         GL_TEXTURE_2D, g_buff_textures[i], 0);
   }

   GenDepthDeferredTexture(&depth_texture_id, width, height);

   GLenum draw_buffers[] = {  GL_COLOR_ATTACHMENT0, 
                              GL_COLOR_ATTACHMENT1,
                              GL_COLOR_ATTACHMENT2,
                              GL_COLOR_ATTACHMENT4  };

   glDrawBuffers(NUM_DEF_TEX, draw_buffers);
   
   assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
   return texture_id;
}

FrameBufferObject::FrameBufferObject(unsigned int width, unsigned int height, 
      TextureSlot texture_slot) :
   position_texture_(static_cast<GLTextureID>(initialize(width, height, 
               DeferredType::POS)), texture_slot),
   normal_texture_(static_cast<GLTextureID>(initialize(width, height, 
               DeferredType::NORM)), texture_slot),
{
}

void FrameBufferObject::bind() {
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id_);
}
