#ifndef GROUND_PLANE_H_
#define GROUND_PLANE_H_

#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "graphics/texture.h"

struct Shader;
struct Shaders;

struct GroundPlane {
   static const int GROUND_SCALE;
   GroundPlane(AttributeLocationMap& locations);
   Mesh mesh_;
   Texture texture_;
   void draw(Shader& shader, const UniformLocationMap& uniform_locations,
             const glm::mat4& viewMatrix);
};

#endif // GROUND_PLANE_H_
