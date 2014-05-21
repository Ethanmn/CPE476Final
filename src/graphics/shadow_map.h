#ifndef SHADOW_MAP_
#define SHADOW_MAP_

#include "gl_adapters/gl_types.h"

class ShadowMapFBO
{
    public:
        ShadowMapFBO(unsigned int WindowWidth, unsigned int WindowHeight);
        void BindForWriting();
        void BindForReading();
        int texture_id();
    private:
        GLuint fbo_id;
        GLuint shadow_map_texture;
};


#endif
