#ifndef BOUNDING_RECTANGLE_H_
#define BOUNDING_RECTANGLE_H_

#include <boost/optional.hpp>
#include <glm/glm.hpp>

#include "graphics/mesh.h"
#include "graphics/location_maps.h"

struct Shader;
struct Shaders;

struct BoundingRectangle {
   BoundingRectangle(
         const glm::vec2& center,
         const glm::vec2& dimensions) :
      center_(center),
      dimensions_(dimensions) {}

   static void loadBoundingMesh(const AttributeLocationMap& locations);

   void set_position(const glm::vec2& center) { center_ = center; }
   void draw(const UniformLocationMap& uniform_locations, Shader& shader, float y) const;

  private:
   glm::vec2 center_;
   glm::vec2 dimensions_;
   static boost::optional<Mesh> bounding_mesh_;
};

#endif // BOUNDING_RECTANGLE_H_
