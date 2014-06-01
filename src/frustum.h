#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include <boost/optional.hpp>
#include <glm/glm.hpp>

#include "graphics/draw_template.h"

struct Frustum {
   enum class TestResult {OUTSIDE, INTERSECT, INSIDE};
   TestResult testSphere(const glm::vec3 &center, float ratio);
   Frustum(const glm::mat4& view_projection);

   std::vector<CulledDrawable> cullDrawables(const std::vector<Drawable>& drawables);

  private:
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
   boost::optional<Plane> planes_[6];
};
#endif //FRUSTUM_H_
