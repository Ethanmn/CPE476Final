#include "frame_buffer_object.h"

#include <stdio.h>
#include <assert.h>

GLuint FrameBufferObject::initialize(unsigned int width, unsigned int height) {
   glGenFramebuffers(1, &fbo_id_);

   GLuint texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, 
         height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
   glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
         GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0);

   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);

   assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
   return texture_id;
}

FrameBufferObject::FrameBufferObject(unsigned int width, unsigned int height, TextureSlot texture_slot) :
   texture_(static_cast<GLTextureID>(initialize(width, height)), texture_slot)
{
}

void FrameBufferObject::bind() {
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id_);
}

void FrameBufferObject::BindForReading() {
   texture_.enable();
}
