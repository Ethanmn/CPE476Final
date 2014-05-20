#ifndef BOUNDING_RECTANGLE_H_
#define BOUNDING_RECTANGLE_H_

#include <boost/optional.hpp>
#include <glm/glm.hpp>

#include "graphics/mesh.h"
#include "graphics/location_maps.h"
#include "graphics/draw_template.h"

struct MeshLoader;
struct Shader;
struct Shaders;

// Bounding rectangle on the XZ plane, with Y rotation.
struct BoundingRectangle {
   BoundingRectangle(
         const glm::vec2& center,
         const glm::vec2& dimensions,
         float y_rotation) :
      center_(center),
      dimensions_(dimensions),
      y_rotation_(y_rotation)
   {}

   static void loadBoundingMesh(MeshLoader& mesh_loader, const AttributeLocationMap& locations);

   bool collidesWith(const BoundingRectangle& other) const;

   void set_position(const glm::vec2& center) { center_ = center; }
   void set_rotation(const float y_rotation) { y_rotation_ = y_rotation; }
   glm::vec2 getCenter();
   glm::vec2 getDimensions();

   static DrawTemplate draw_template();
   glm::mat4 model_matrix() const;

  private:
   std::vector<glm::vec2> corners() const;
   std::vector<float> corner_projections(const glm::vec2& separating_axis) const;

   bool hasSeparatingLineForAxis(
         const glm::vec2& separating_axis,
         const BoundingRectangle& other) const;

   glm::vec2 center_;
   glm::vec2 dimensions_;

   float y_rotation_;

   static boost::optional<Mesh> bounding_mesh_;
};

#endif // BOUNDING_RECTANGLE_H_
