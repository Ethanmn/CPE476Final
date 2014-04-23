/* 
   DeerCam.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Camera.h"

struct DeerCam : public Camera {
   DeerCam(float lookAtHeight = 0);

   void move(glm::vec3 deerPos);
   void setLookAtHeight(float height);
   void initialize(glm::vec3 deerPos);

   private:
      float lookAtHeight;
};
