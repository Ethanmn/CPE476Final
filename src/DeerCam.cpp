/* 
   DeerCam.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "DeerCam.h"
#include <iostream>

const float DIST_FROM_DEER = 20;

DeerCam::DeerCam(float lookHeight) {
   lookAtHeight = lookHeight;
}

void DeerCam::initialize(glm::vec3 deerPos) {
   lookAtHeight = deerPos.y + 4;
   setLookAt(deerPos);
   updatePosition(DIST_FROM_DEER);
}

void DeerCam::move(glm::vec3 deerPos) {
   setLookAt(glm::vec3(deerPos.x, lookAtHeight, deerPos.z));
   updatePosition(DIST_FROM_DEER);
}

void DeerCam::setLookAtHeight(float height) {
   glm::vec3 lookAt = getLookAt();
   lookAtHeight = height;
   setLookAt(glm::vec3(lookAt.x, lookAtHeight, lookAt.z));
}
