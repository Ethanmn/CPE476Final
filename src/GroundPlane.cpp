#include "GroundPlane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/shaders.h"

const std::vector<float> ground_vertices{
   -0.5, 0.0, -0.5,
   0.5, 0.0, -0.5,
   -0.5, 0.0, 0.5,
   0.5, 0.0, 0.5,
};
const std::vector<float> ground_normals{
   0, 0, 1,
   0, 0, 1,
   0, 0, 1,
   0, 0, 1,
};
const std::vector<float> ground_tex_coord{
   0.0, 0.0,
   1.0, 0.0,
   0.0, 1.0,
   1.0, 1.0
};

const std::vector<unsigned short> ground_indices{
   0, 2, 1, 3, 1, 2
};

GroundPlane::GroundPlane(AttributeLocationMap locations, Shaders& shaders) :
   mesh_{
      IndexBufferObject::create(ground_indices),
      {
         ArrayBufferObject::create(
            ground_vertices,
            locations[Attribute::VERTEX],
            3),
         ArrayBufferObject::create(
            ground_normals,
            locations[Attribute::NORMAL],
            3),
         ArrayBufferObject::create(
            ground_tex_coord,
            locations[Attribute::TEX_COORD],
            2),
      }
   }
{}

void GroundPlane::draw(Shader& shader, const UniformLocationMap& uniform_locations) {
   glm::mat4 transform = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)) *
      glm::scale(glm::mat4(1.0), glm::vec3(30.0));
   shader.sendUniform(Uniform::MODEL, uniform_locations, transform);
   shader.sendUniform(Uniform::COLOR, uniform_locations, glm::vec4(0, 1, 0, 0.5f));
   shader.drawMesh(mesh_);
}
