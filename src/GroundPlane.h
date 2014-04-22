#ifndef GROUND_PLANE_H_
#define GROUND_PLANE_H_

#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/uniform_location_map.h"

struct Shader;
struct Shaders;

struct GroundPlane {
   GroundPlane(Shaders& shaders);
   Mesh mesh_;
   void draw(Shader& shader);
};

#endif // GROUND_PLANE_H_
