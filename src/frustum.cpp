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

std::vector<CulledDrawable> Frustum::cullShadowDrawables(std::vector<CulledDrawable> drawables) {
   for (auto& drawable : drawables) {
      for (auto& instance : drawable.draw_instances) {
         if (testSphere(BoundingSphere(drawable.draw_template, instance.model.model_transform)) == Frustum::TestResult::OUTSIDE) {
            instance.cullFlag.insert(CullType::SHADOW_CULLING);
         }
      }
   }
   return drawables;
}

std::vector<CulledDrawable> Frustum::cullDrawables(const std::vector<Drawable>& drawables) {
   int culledObject = 0;

   std::vector<CulledDrawable> culledDrawables;
   for (auto& drawable : drawables) {
      CulledDrawable culledDrawable;
      for (auto& instance : drawable.draw_instances) {
         CulledDrawInstance culledTransform;
         culledTransform.model = instance;

         if (testSphere(BoundingSphere(drawable.draw_template, instance.model_transform)) == Frustum::TestResult::OUTSIDE) {
            culledTransform.cullFlag.insert(CullType::VIEW_CULLING);
            culledObject++;
         }
         culledDrawable.draw_instances.push_back(culledTransform);
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

Frustum::TestResult Frustum::testSphere(const BoundingSphere& sphere) {
   TestResult result = TestResult::INSIDE;
   for (int i = 0; i < 6; i++) {
      if (!planes_[i]) continue;
      const auto distance = planes_[i]->distance(sphere.center);
      if (distance < -sphere.radius)
         return TestResult::OUTSIDE;
      else if (distance < sphere.radius)
         result = TestResult::INTERSECT;
   }
   return result;
}
