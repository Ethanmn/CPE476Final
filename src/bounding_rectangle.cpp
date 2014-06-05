#include "bounding_rectangle.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "graphics/assimp/mesh_loader.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "globals.h"
namespace {
   glm::vec2 vec2FromAngle(float y_rotation) {
      return glm::rotate(glm::vec2(1, 0), 360.0f - y_rotation);
   }
}

//static
boost::optional<Mesh> BoundingRectangle::bounding_mesh_ = boost::none;

//static
void BoundingRectangle::loadBoundingMesh(MeshLoader& mesh_loader, const AttributeLocationMap& locations) {
   bounding_mesh_ = Mesh::fromAssimpMesh(locations, mesh_loader.loadMesh(MeshType::BOX));
}

//static
DrawTemplate BoundingRectangle::draw_template() {
   return DrawTemplate({
         ShaderType::TEXTURE,
         *bounding_mesh_,
         boost::none,
         boost::none,
         EffectSet(),
         });
}

glm::mat4 BoundingRectangle::model_matrix() const {
   const auto orient(
         glm::rotate(
            glm::mat4(),
            -90.0f,
            glm::vec3(1, 0, 0)));
   const auto rotate(
         glm::rotate(
            glm::mat4(),
            y_rotation_,
            glm::vec3(0, 1, 0)));
   const auto scale(
         glm::scale(
            glm::mat4(),
            glm::vec3(dimensions_.x / 4.0f, 0.01f, dimensions_.y / 4.0f))); // divide by 4 for the box model
   const auto translate(
         glm::translate(
            glm::mat4(),
            glm::vec3(center_.x, 5.0f, center_.y)));
   return translate * rotate * scale * orient;
}

glm::mat4 BoundingRectangle::model_matrix_screen() const {
   const int dimScale = 400.f;
   const auto orient(
         glm::rotate(
            glm::mat4(),
            90.0f,
            glm::vec3(1, 0, 0)));
   const auto rotate(
         glm::rotate(
            glm::mat4(),
            90.0f,
            glm::vec3(0, 1, 0)));
   const auto scale(
         glm::scale(
            glm::mat4(),
            glm::vec3(dimensions_.x / dimScale, 
            dimensions_.y / dimScale,
            dimensions_.x / dimScale)));
   const auto translate(
         glm::translate(
            glm::mat4(),
            glm::vec3(0.0f, 0.0f, 0.0f)));
   return translate * scale * rotate * orient;
}

bool BoundingRectangle::collidesWith(const BoundingRectangle& other) const {
   auto has_separation = hasSeparatingLineForAxis(vec2FromAngle(y_rotation_), other);
   has_separation = has_separation || hasSeparatingLineForAxis(vec2FromAngle(y_rotation_ + 90), other);
   has_separation = has_separation || hasSeparatingLineForAxis(vec2FromAngle(other.y_rotation_), other);
   has_separation = has_separation || hasSeparatingLineForAxis(vec2FromAngle(other.y_rotation_ + 90), other);
   return !has_separation;
}

bool BoundingRectangle::hasSeparatingLineForAxis(
      const glm::vec2& separating_axis,
      const BoundingRectangle& other) const {
   const auto projections_a = corner_projections(separating_axis);
   float min_a = *std::min_element(projections_a.begin(), projections_a.end());
   float max_a = *std::max_element(projections_a.begin(), projections_a.end());

   const auto projections_b = other.corner_projections(separating_axis);
   float min_b = *std::min_element(projections_b.begin(), projections_b.end());
   float max_b = *std::max_element(projections_b.begin(), projections_b.end());

   return min_a > max_b || min_b > max_a;
}

std::vector<float> BoundingRectangle::corner_projections(const glm::vec2& separating_axis) const {
   std::vector<float> corner_projections;
   for (const auto& corner : corners()) {
      corner_projections.push_back(glm::dot(corner, separating_axis));
   }
   return corner_projections;
}

std::vector<glm::vec2> BoundingRectangle::corners() const {
   return {
      center_ + glm::rotate(glm::vec2(dimensions_.x / 2, dimensions_.y / 2), 360.0f - y_rotation_),
      center_ + glm::rotate(glm::vec2(dimensions_.x / 2, -dimensions_.y / 2), 360.0f - y_rotation_),
      center_ + glm::rotate(glm::vec2(-dimensions_.x / 2, dimensions_.y / 2), 360.0f - y_rotation_),
      center_ + glm::rotate(glm::vec2(-dimensions_.x / 2, -dimensions_.y / 2), 360.0f - y_rotation_),
   };
}

glm::vec2 BoundingRectangle::getCenter() {
   return center_;
}

glm::vec2 BoundingRectangle::getDimensions() {
   return dimensions_;
}
