#ifndef UNIFORM_LOCATION_MAP_H_
#define UNIFORM_LOCATION_MAP_H_

#include "graphics/gl_adapters/gl_types.h"
#include "graphics/uniforms.h"

struct UniformLocationMap {
   Uniform uniform;
   GLUniformLocationMap location_map;
};

#endif // UNIFORM_LOCATION_MAP_H_
