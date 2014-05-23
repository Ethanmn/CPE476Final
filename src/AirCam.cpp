/* 
   AirCam.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "AirCam.h"
#include <iostream>

const float DIST_FROM_DEER = 50;

AirCam::AirCam(float lookHeight) {
   lookAtHeight = lookHeight;
}

void AirCam::initialize(glm::vec3 deerPos) {
   lookAtHeight = deerPos.y;
   setLookAt(deerPos);
   updatePosition(DIST_FROM_DEER);
}

void AirCam::move(glm::vec3 deerPos) {
   setLookAt(glm::vec3(deerPos.x, lookAtHeight, deerPos.z));
   updatePosition(DIST_FROM_DEER);
}

void AirCam::setLookAtHeight(float height) {
   glm::vec3 lookAt = getLookAt();
   lookAtHeight = height;
   setLookAt(glm::vec3(lookAt.x, lookAtHeight, lookAt.z));
}
