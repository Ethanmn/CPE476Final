#include "frustum.h"
#include <iostream>

namespace {
   Plane constructPlane(const glm::vec4& coefficients) {
      return Plane(
            glm::vec3(coefficients),
            coefficients[3]
            );
   }
}

Frustum::Frustum(const glm::mat4& vp) {
   const auto transpose = glm::transpose(vp);
   planes_[LEFT] = constructPlane(transpose[3] + transpose[0]);
   planes_[RIGHT] = constructPlane(transpose[3] - transpose[0]);

   planes_[BOTTOM] = constructPlane(transpose[3] + transpose[1]);
   planes_[TOP] = constructPlane(transpose[3] - transpose[1]);

   planes_[NEARP] = constructPlane(transpose[3] + transpose[2]);
   planes_[FARP] = constructPlane(transpose[3] - transpose[2]);
}

Frustum::TestResult Frustum::testSphere(const glm::vec3 &p, float radius) {
   TestResult result = TestResult::INSIDE;
   for (int i = 0; i < 6; i++) {
      if (!planes_[i]) continue;
      const auto distance = planes_[i]->distance(p);
      if (distance < -radius)
         return TestResult::OUTSIDE;
      else if (distance < radius)
         result = TestResult::INTERSECT;
   }
   return result;
}
