#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include <boost/optional.hpp>
#include <glm/glm.hpp>
#include "plane.h"

struct Frustum {
   enum class TestResult {OUTSIDE, INTERSECT, INSIDE};
   TestResult testSphere(glm::vec3 &p, float ratio);
   Frustum(const glm::mat4& view_projection);

   void setCamInternals(float angle, float ratio, float nearD, float farD);
   void setCamDef(const glm::vec3 &p, const glm::vec3 &l, const glm::vec3 &u);

  private:
   enum {
      TOP = 0, BOTTOM, LEFT,
      RIGHT, NEARP, FARP
   };
   boost::optional<Plane> planes_[6];
   glm::vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
   float nearD, farD, ratio, angle,tang;
   float nw,nh,fw,fh;
};
#endif //FRUSTUM_H_
