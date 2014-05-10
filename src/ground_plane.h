#ifndef GROUND_PLANE_H_
#define GROUND_PLANE_H_

#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"

struct Shader;
struct Shaders;

struct GroundPlane {
   static const int GROUND_SCALE;
   GroundPlane(const Mesh& mesh);
   void draw(Shader& shader, const UniformLocationMap& uniform_locations,
             const glm::mat4& viewMatrix);

  private:
   Texture texture_, height_map_;
   Mesh mesh_;
};

#endif // GROUND_PLANE_H_
