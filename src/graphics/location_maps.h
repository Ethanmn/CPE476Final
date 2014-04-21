#ifndef LOCATION_MAPS_H_
#define LOCATION_MAPS_H_

#include "graphics/attributes.h"
#include "graphics/gl_adapters/gl_types.h"
#include "graphics/uniforms.h"

typedef std::map<Uniform, GLUniformLocationMap> UniformLocationMap;
typedef std::map<Attribute, GLAttributeLocationMap> AttributeLocationMap;

#endif // LOCATION_MAPS_H_
