#ifndef SHADOW_MAP_
#define SHADOW_MAP_

#include "gl_adapters/gl_types.h"

class ShadowMapFBO
{
    public:
        ShadowMapFBO();
        bool setup(unsigned int WindowWidth, unsigned int WindowHeight);
        void BindForWriting();
        void BindForReading();
    private:
        GLuint fbo_id;
        GLuint shadow_map_texture;
};


#endif