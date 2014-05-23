/* 
   AirCam.cpp
   Katie Keim
   Deer - CPE 476
*/
#ifndef AIR_CAM_H_
#define AIR_CAM_H_

#include "Camera.h"

struct AirCam : public Camera {
   AirCam(float lookAtHeight = 0);

   void move(glm::vec3 deerPos);
   void setLookAtHeight(float height);
   void initialize(glm::vec3 deerPos);

   private:
      float lookAtHeight;
};

#endif //AIR_CAM_H_
