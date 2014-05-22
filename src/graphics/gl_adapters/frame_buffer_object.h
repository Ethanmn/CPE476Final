#ifndef FRAME_BUFFER_OBJECT_H_
#define FRAME_BUFFER_OBJECT_H_

#include "graphics/gl_adapters/gl_types.h"

struct FrameBufferObject {
   FrameBufferObject(unsigned int width, unsigned int height);

   void BindForWriting();
   void BindForReading();
   int texture_id();

  private:
   GLuint fbo_id;
   GLuint texture_id_;
};

#endif // FRAME_BUFFER_OBJECT_H_
