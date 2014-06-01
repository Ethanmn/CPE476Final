#include "frustum.h"
#include <iostream>

#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum(const glm::mat4& vp) {
   planes_[LEFT] = Plane(
         glm::vec3(
            vp[3][0] + vp[0][0],
            vp[3][1] + vp[0][1],
            vp[3][2] + vp[0][2]
            ),
         vp[3][3] + vp[0][3]
         );

   planes_[RIGHT] = Plane(
         glm::vec3(
            vp[3][0] - vp[0][0],
            vp[3][1] - vp[0][1],
            vp[3][2] - vp[0][2]
            ),
         vp[3][3] - vp[0][3]
         );

   planes_[TOP] = Plane(
         glm::vec3(
            vp[3][0] - vp[1][0],
            vp[3][1] - vp[1][1],
            vp[3][2] - vp[1][2]
            ),
         vp[3][3] - vp[1][3]
         );

   planes_[BOTTOM] = Plane(
         glm::vec3(
            vp[3][0] + vp[1][0],
            vp[3][1] + vp[1][1],
            vp[3][2] + vp[1][2]
            ),
         vp[3][3] + vp[1][3]
         );

   planes_[NEARP] = Plane(
         glm::vec3(
            vp[3][0] + vp[2][0],
            vp[3][1] + vp[2][1],
            vp[3][2] + vp[2][2]
            ),
         vp[3][3] + vp[2][3]
         );

   planes_[FARP] = Plane(
         glm::vec3(
            vp[3][0] - vp[2][0],
            vp[3][1] - vp[2][1],
            vp[3][2] - vp[2][2]
            ),
         vp[3][3] - vp[2][3]
         );
}

void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD) {
   // store the information
   this->ratio = ratio;
   this->angle = angle;
   this->nearD = nearD;
   this->farD = farD;

   // compute width and height of the near and far plane sections
   tang = (float)tan(ANG2RAD * angle * 0.5);
   nh = nearD * tang;
   nw = nh * ratio;
   fh = farD * tang;
   fw = fh * ratio;
}

void Frustum::setCamDef(const glm::vec3 &p, const glm::vec3 &l, const glm::vec3 &u) {

   glm::vec3 dir, nc, fc, X, Y, Z;

   // compute the Z axis of camera
   // this axis points in the opposite direction from
   // the looking direction
   Z = glm::normalize(p - l);

   // X axis of camera with given "up" vector and Z axis
   X = glm::normalize(glm::cross(u, Z));

   // the real "up" vector is the cross product of Z and X
   Y = glm::normalize(glm::cross(Z, X));

   // compute the centers of the near and far planes
   nc = p - Z * nearD;
   fc = p - Z * farD;

   // compute the 4 corners of the frustum on the near plane
   ntl = nc + Y * nh - X * nw;
   ntr = nc + Y * nh + X * nw;
   nbl = nc - Y * nh - X * nw;
   nbr = nc - Y * nh + X * nw;

   // compute the 4 corners of the frustum on the far plane
   ftl = fc + Y * fh - X * fw;
   ftr = fc + Y * fh + X * fw;
   fbl = fc - Y * fh - X * fw;
   fbr = fc - Y * fh + X * fw;

   // compute the six planes
   // the function Plane assumes that the points
   // are given in counter clockwise order
   planes_[TOP] = Plane(ntr,ntl,ftl);
   planes_[BOTTOM] = Plane(nbl,nbr,fbr);
   planes_[LEFT] = Plane(ntl,nbl,fbl);
   planes_[RIGHT] = Plane(nbr,ntr,fbr);
   planes_[NEARP] = Plane(ntl,ntr,nbr);
   planes_[FARP] = Plane(ftr,ftl,fbl);
}

Frustum::TestResult Frustum::testSphere(glm::vec3 &p, float radius) {
   float distance;
   TestResult result = TestResult::INSIDE;

   for (int i = 0; i < 6; i++) {
      distance = planes_[i]->distance(p);
      if (distance < -radius)
         return TestResult::OUTSIDE;
      else if (distance < radius)
         result = TestResult::INTERSECT;
   }
   return result;
}
