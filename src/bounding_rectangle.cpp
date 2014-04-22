#include "bounding_rectangle.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "graphics/assimp/mesh_loader.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"

namespace {
   glm::vec2 vec2FromAngle(float y_rotation) {
      return glm::rotate(glm::vec2(1, 0), y_rotation);
   }
}

//static
boost::optional<Mesh> BoundingRectangle::bounding_mesh_ = boost::none;

//static
void BoundingRectangle::loadBoundingMesh(const AttributeLocationMap& locations) {
   bounding_mesh_ = Mesh::fromAssimpMesh(locations, loadMesh("../models/cube.obj"));
}

void BoundingRectangle::draw(const UniformLocationMap& locations, Shader& shader, float y) const {
   if (bounding_mesh_) {
      const glm::mat4 rotate(
            glm::rotate(
               glm::mat4(),
               y_rotation_,
               glm::vec3(0, 1, 0)));
      glm::mat4 scale(
            glm::scale(
               glm::mat4(),
               glm::vec3(dimensions_.x / 2.0f, 0.01f, dimensions_.y / 2.0f)));
      glm::mat4 translate(
            glm::translate(
               glm::mat4(),
               glm::vec3(center_.x, y, center_.y)));

      shader.sendUniform(Uniform::MODEL, locations, translate * rotate * scale);
      shader.drawMesh(*bounding_mesh_);
   }
}

bool BoundingRectangle::collidesWith(const BoundingRectangle& other) const {
   const auto radius_axis = center_ - other.center_;
   auto has_separation = hasSeparatingLineForAxis(vec2FromAngle(y_rotation_), radius_axis, other);
   has_separation = has_separation || hasSeparatingLineForAxis(vec2FromAngle(y_rotation_ + 90), radius_axis, other);
   has_separation = has_separation || hasSeparatingLineForAxis(vec2FromAngle(other.y_rotation_), radius_axis, other);
   has_separation = has_separation || hasSeparatingLineForAxis(vec2FromAngle(other.y_rotation_ + 90), radius_axis, other);
   return !has_separation;
}

glm::vec2 BoundingRectangle::localX() const {
   return vec2FromAngle(y_rotation_) * dimensions_.x / 2.0f;
}

glm::vec2 BoundingRectangle::localZ() const {
   return vec2FromAngle(y_rotation_ + 90) * dimensions_.y / 2.0f;
}

float BoundingRectangle::total_projection(const glm::vec2& separating_axis) const {
   return std::abs(glm::dot(localX(), separating_axis)) + std::abs(glm::dot(localZ(), separating_axis));
}

bool BoundingRectangle::hasSeparatingLineForAxis(
      const glm::vec2& separating_axis,
      const glm::vec2& radius_axis,
      const BoundingRectangle& other) const {
   return std::abs(glm::dot(separating_axis, radius_axis)) >
      total_projection(separating_axis) + other.total_projection(separating_axis);
}
