#ifndef BOUNDING_RECTANGLE_H_
#define BOUNDING_RECTANGLE_H_

#include <boost/optional.hpp>
#include <glm/glm.hpp>

#include "graphics/mesh.h"
#include "graphics/uniform_location_map.h"

struct Shader;
struct Shaders;

struct BoundingRectangle {
   BoundingRectangle(
         const glm::vec2& position,
         const glm::vec2& dimensions) :
      position_(position),
      dimensions_(dimensions) {}

   static void loadBoundingMesh(Shaders& shaders);

   float left() const  { return position_.x; }
   float right() const { return position_.x + dimensions_.x; }
   float front() const { return position_.y; }
   float back() const  { return position_.y + dimensions_.y; }

   bool collidesWith(const BoundingRectangle& other) const {
      return right() >= other.left() &&
             left() <= other.right() &&
             front() <= other.back() &&
             back() >= other.front();
   }

   void set_position(const glm::vec2& position) { position_ = position; }
   void draw(const UniformLocationMap& model_locations, Shader& shader, float y) const;

  private:
   glm::vec2 position_;
   glm::vec2 dimensions_;
   static boost::optional<Mesh> bounding_mesh_;
};

#endif // BOUNDING_RECTANGLE_H_
