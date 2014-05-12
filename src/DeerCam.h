/* 
   DeerCam.cpp
   Katie Keim
   Deer - CPE 476
*/
#ifndef DEER_CAM_H_
#define DEER_CAM_H_

#include "Camera.h"

struct DeerCam : public Camera {
   DeerCam(float lookAtHeight = 0);

   void move(glm::vec3 deerPos);
   void setLookAtHeight(float height);
   void initialize(glm::vec3 deerPos);

   private:
      float lookAtHeight;
};

#endif //DEER_CAM_H_
