#include "tree.h"

#include <glm/gtc/matrix_transform.hpp>

#include "graphics/shader.h"

void Tree::draw(
      Shader& shader,
      const UniformLocationMap& uniform_location_map,
      const glm::mat4& view_matrix) const {
   const glm::mat4 translate(glm::translate(
            glm::mat4(),
            position_));
   const glm::mat4 rotate(glm::rotate(
            glm::mat4(),
            -90.0f,
            glm::vec3(1, 0, 0)));
   const glm::mat4 model_matrix(translate * rotate);
   shader.sendUniform(Uniform::MODEL, uniform_location_map, model_matrix);
   shader.drawMesh(mesh_);
}
