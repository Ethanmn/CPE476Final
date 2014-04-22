/* 
   DeerCam.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "DeerCam.h"

const float deerCamHeightAbvDeer = 8; 
const float deerCamDistFromDeer = 16;

void DeerCam::move(glm::vec3 deerPos) {
   const glm::vec3 camDir = getCamForwardVec();

   float camX = deerPos.x - (camDir.x * deerCamDistFromDeer);
   float camY = deerPos.y + deerCamHeightAbvDeer;
   float camZ = deerPos.z - (camDir.z * deerCamDistFromDeer);

   setPosition(glm::vec3(camX, camY, camZ));
   setLookAt(deerPos);
}
