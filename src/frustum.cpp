#include "frustum.h"
#include <iostream>

Frustum::Plane::Plane(const glm::vec4& coefficients) :
   normal_(coefficients),
   D(coefficients[3])
{
   float mag = glm::length(normal_);
   normal_ /= mag;
   D /= mag;
}

std::vector<CulledDrawable> Frustum::cullDrawables(const std::vector<Drawable>& drawables) {
   int culledObject = 0;

   std::vector<CulledDrawable> culledDrawables;
   for (auto& drawable : drawables) {
      CulledDrawable culledDrawable;
      for (auto& transform : drawable.model_transforms) {
         CulledTransform culledTransform;
         culledTransform.model = transform;

         const auto min = glm::vec3(transform * glm::vec4(drawable.draw_template.mesh.min, 1));
         const auto max = glm::vec3(transform * glm::vec4(drawable.draw_template.mesh.max, 1));
         const auto mid = (min + max) / 2.0f;

         if (testSphere(mid, glm::length(max - mid)) == Frustum::TestResult::OUTSIDE) {
            culledTransform.cullFlag.insert(CullType::VIEW_CULLING);
            culledObject++;
         }
         culledDrawable.model_transforms.push_back(culledTransform);
      }
      culledDrawable.draw_template = drawable.draw_template;
      culledDrawables.push_back(culledDrawable);
   }
   return culledDrawables;
}

float Frustum::Plane::distance(const glm::vec3& p) {
   return glm::dot(normal_, p) + D;
}

Frustum::Frustum(const glm::mat4& view_projection) {
   const auto transpose = glm::transpose(view_projection);
   planes_[LEFT] = Plane(transpose[3] + transpose[0]);
   planes_[RIGHT] = Plane(transpose[3] - transpose[0]);

   planes_[BOTTOM] = Plane(transpose[3] + transpose[1]);
   planes_[TOP] = Plane(transpose[3] - transpose[1]);

   planes_[NEARP] = Plane(transpose[3] + transpose[2]);
   planes_[FARP] = Plane(transpose[3] - transpose[2]);
}

Frustum::TestResult Frustum::testSphere(const glm::vec3 &center, float radius) {
   TestResult result = TestResult::INSIDE;
   for (int i = 0; i < 6; i++) {
      if (!planes_[i]) continue;
      const auto distance = planes_[i]->distance(center);
      if (distance < -radius)
         return TestResult::OUTSIDE;
      else if (distance < radius)
         result = TestResult::INTERSECT;
   }
   return result;
}
