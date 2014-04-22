#ifndef GROUND_PLANE_H_
#define GROUND_PLANE_H_

#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"

struct Shader;
struct Shaders;

struct GroundPlane {
   GroundPlane(AttributeLocationMap& locations);
   Mesh mesh_;
   void draw(Shader& shader, const UniformLocationMap& uniform_locations);
};

#endif // GROUND_PLANE_H_
