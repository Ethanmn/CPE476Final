#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include <glm/glm.hpp>
#include "plane.h"

struct Frustum {

   private:

      enum {
         TOP = 0, BOTTOM, LEFT,
         RIGHT, NEARP, FARP
      };
      Plane pl[6];
      glm::vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
      float nearD, farD, ratio, angle,tang;
      float nw,nh,fw,fh;

   public:

      enum {OUTSIDE, INTERSECT, INSIDE};

      void setCamInternals(float angle, float ratio, float nearD, float farD);
      void setCamDef(const glm::vec3 &p, const glm::vec3 &l, const glm::vec3 &u);
      int sphereInFrustum(glm::vec3 &p, float ratio);
};
#endif //FRUSTUM_H_
