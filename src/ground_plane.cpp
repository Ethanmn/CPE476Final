#include "ground_plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/shader_setup.h"

const int GroundPlane::GROUND_SCALE = 500;

const std::vector<unsigned short> ground_indices{
   0, 2, 1, 3, 1, 2
};

GroundPlane::GroundPlane(const Mesh& mesh) :
   texture_(texture_path(Textures::GRASS)),
   height_map_(texture_path(Textures::HEIGHT_MAP)),
   mesh_(mesh)
      {}

void GroundPlane::draw(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupTextureShader(shader, uniform_locations, texture_);
   const glm::mat4 translate(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -7.0, 0.0)));
   const glm::mat4 rotate(glm::rotate(glm::mat4(1.0), 90.0f, glm::vec3(1, 0, 0)));
   const glm::mat4 scale(glm::scale(glm::mat4(1.0), glm::vec3(GROUND_SCALE / 50)));
   const glm::mat4 transform = translate * rotate * scale;

   setupModelView(shader, uniform_locations, transform, viewMatrix, true);
   shader.drawMesh(mesh_);
   texture_.disable();
}
