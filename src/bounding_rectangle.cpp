#include "bounding_rectangle.h"

#include <glm/gtc/matrix_transform.hpp>

#include "graphics/assimp/mesh_loader.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"

//static
boost::optional<Mesh> BoundingRectangle::bounding_mesh_ = boost::none;

//static
void BoundingRectangle::loadBoundingMesh(const AttributeLocationMap& locations) {
   bounding_mesh_ = Mesh::fromAssimpMesh(locations, loadMesh("../models/cube.obj"));
}

void BoundingRectangle::draw(const UniformLocationMap& locations, Shader& shader, float y) const {
   if (bounding_mesh_) {
      glm::mat4 model_matrix(
            glm::scale(
               glm::translate(glm::mat4(1.0f), glm::vec3(center_.x, y, center_.y)),
               glm::vec3(dimensions_.x, 0.01f, dimensions_.y)));
      shader.sendUniform(Uniform::COLOR, locations, glm::vec4(1.0f, 0, 0, 1.0f));
      shader.sendUniform(Uniform::MODEL, locations, model_matrix);
      shader.drawMesh(*bounding_mesh_);
   }
}
