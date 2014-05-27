/* 
   Camera.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 look) :
   position(pos),
   lookAt(look),
   up(glm::vec3(0.0f, 1.0f, 0.0f)),
   target(pos),
   springStrength(0.075f),
   dampConst(0.0065f)
{}

/* 'Jumps' the camera to the specified position. */
void Camera::setPosition(const glm::vec3& endPosition) {
   position = endPosition;
}

/* Changes the point the camera is looking at. */
void Camera::setLookAt(const glm::vec3& lookAtPoint) {
   lookAt = glm::vec3(lookAtPoint.x, lookAtPoint.y + 4.0f, lookAtPoint.z);
}

/* Retrieves the current position of the camera. */
glm::vec3 Camera::getPosition() const {
   return position;
}

/* Retrieves the current look at point of the camera. */
glm::vec3 Camera::getLookAt() const {
   return lookAt;
}

/* Retrieves the vector pointing straight in front of the camera. */
glm::vec3 Camera::getCamForwardVec() const {
   return (lookAt - position) / glm::length(lookAt - position);
}

/* Retrieves the vector pointing to the left of the camera. */
glm::vec3 Camera::getCamLeftVec() const {
   glm::vec3 w = getCamForwardVec();
   return glm::cross(up, w) / glm::length(glm::cross(up, w));
}

/* Retrieves the view matrix (glm::mat4) for shaders. */
glm::mat4 Camera::getViewMatrix() const {
   return glm::lookAt(position, lookAt, up);
}

void Camera::step(float dT) {
   glm::vec3 newPos = position;
   glm::vec3 displacement;
   float dispLength = 0.0f;
   float springMag = 0.0f;
   float scalar = 0.0f;

   glm::vec3 camForward = getCamForwardVec();
   if (glm::length(position - lookAt) > 30.0f) {
      target += camForward * (dT / 100.0f) * 10.0f;
   }

   if (glm::length(position - lookAt) < 10.0f) {
      target -=  camForward * (dT / 100.0f) * 10.0f;
   }

   displacement = newPos - target;

   dispLength = glm::length(displacement);

   if (dispLength >= 0.001) {
      springMag = springStrength * dispLength + dampConst * (1 / dispLength);
      scalar = (1.0f / dispLength) * springMag;
      displacement *= scalar;
      newPos.x -= displacement.x;
      newPos.y -= displacement.y;
      newPos.z -= displacement.z;
   }

   position = newPos;

   if (position.y < 1.0f) {
      position.y = 1.0f;
   }

   position.y += 15.0f;
}
