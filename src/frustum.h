#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include <boost/optional.hpp>
#include <glm/glm.hpp>

#include "graphics/draw_template.h"

struct Frustum {
   enum class TestResult {OUTSIDE, INTERSECT, INSIDE};

   Frustum(const glm::mat4& view_projection);

   std::vector<CulledDrawable> cullDrawables(const std::vector<Drawable>& drawables);
   std::vector<CulledDrawable> cullShadowDrawables(std::vector<CulledDrawable> drawables);

  private:
   struct BoundingSphere {
      BoundingSphere(const DrawTemplate& dt, const glm::mat4& model) {
         const auto min = glm::vec3(model * glm::vec4(dt.mesh.min, 1));
         const auto max = glm::vec3(model * glm::vec4(dt.mesh.max, 1));
         center = (min + max) / 2.0f;
         radius = glm::length(max - center);
      }

      glm::vec3 center;
      float radius;
   };

   struct Plane {
      Plane(const glm::vec4& coefficients);
      float distance(const glm::vec3& p);

     private:
      glm::vec3 normal_; // A, B, C
      float D;
   };

   enum {
      TOP = 0, BOTTOM, LEFT,
      RIGHT, NEARP, FARP
   };

   TestResult testSphere(const BoundingSphere& sphere);

   boost::optional<Plane> planes_[6];
};
#endif //FRUSTUM_H_
