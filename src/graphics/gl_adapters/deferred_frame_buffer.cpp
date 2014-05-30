#include "deferred_frame_buffer.h"

#include <stdio.h>
#include <assert.h>
#define NUM_DEF_TEX 2

void GenDeferredTexture(GLuint *texture_id, unsigned int width, unsigned int height);
void GenDepthDeferredTexture(GLuint *texture_id, unsigned int width, unsigned int height);

DeferredFrameBuffer::DeferredFrameBuffer(unsigned int width, unsigned int height) 
{
   Initialize(width, height);
}


void DeferredFrameBuffer::Initialize(unsigned int width, unsigned int height) {
   glGenFramebuffers(1, &fbo_id_);
   glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);

   for(int i = 0; i < NUM_DEF_TEX; i++) {
      GenDeferredTexture(&g_buff_textures[i], width, height);  
      glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
         GL_TEXTURE_2D, g_buff_textures[i], 0);
   }

   GenDepthDeferredTexture(&g_buff_depth_texture, width, height);

   GLenum draw_buffers[] = {  GL_COLOR_ATTACHMENT0, 
                              GL_COLOR_ATTACHMENT1 /*,
                              GL_COLOR_ATTACHMENT2 */  };
   glDrawBuffers(NUM_DEF_TEX, (GLenum*) draw_buffers);
   
   assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


void DeferredFrameBuffer::Bind() {
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id_);
}


void DeferredFrameBuffer::SetBufferToRead(GBufferType buffType) {
   glReadBuffer(GL_COLOR_ATTACHMENT0 + (int)buffType);
}



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
