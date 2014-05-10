#include "ground_plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/shader_setup.h"

const int GroundPlane::GROUND_SCALE = 500;

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
   0.0, 0.0, 0.0,
   10.0, 0.0, 0.0,
   0.0, 10.0, 0.0,
   10.0, 10.0, 0.0
};

const std::vector<unsigned short> ground_indices{
   0, 2, 1, 3, 1, 2
};

GroundPlane::GroundPlane(AttributeLocationMap& locations) :
   mesh_({
      IndexBufferObject::create(ground_indices),
      std::vector<ArrayBufferObject>({
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
            3),
      }),
      std::vector<Bone>(),
      Animation(),
      Material(),
   }),
   texture_(texture_path(Textures::GRASS)) {}

void GroundPlane::draw(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupTextureShader(shader, uniform_locations, texture_);
   glm::mat4 transform = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -7.0, 0.0)) *
      glm::scale(glm::mat4(1.0), glm::vec3(GROUND_SCALE));

   setupModelView(shader, uniform_locations, transform, viewMatrix, true);
   shader.drawMesh(mesh_);
   texture_.disable();
}
