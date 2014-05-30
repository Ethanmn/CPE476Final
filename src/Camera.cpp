/* 
   Camera.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Camera.h"
#include <iostream>

//Height offsets
const float LOOK_AT_HEIGHT = 4.0f;
const float POSITION_HEIGHT = 15.0f;

//The camera will only move when outside these distances from the deer
const float MIN_DIST_FROM_DEER = 10.0f;
const float MAX_DIST_FROM_DEER = 30.0f;

Camera::Camera(glm::vec3 pos, glm::vec3 look) :
   position(pos),
   lookAt(look),
   up(glm::vec3(0.0f, 1.0f, 0.0f)),
   target(pos),
   springStrength(0.128f), //Larger = stiffer spring
   dampConst(0.0065f),     //Larger = lighter mass (more spring motion),
   speed(7.5f)             //Larger = faster spring movement
{}

/* Sets the camera to the specified position. BETTER TO SET THE LOOK AT! */
void Camera::setPosition(const glm::vec3& endPosition) {
   position = endPosition;
}

/* Changes the point the camera is looking at. It will set the y to slightly above what is passed in. */
void Camera::setLookAt(const glm::vec3& lookAtPoint) {
   lookAt = glm::vec3(lookAtPoint.x, lookAtPoint.y + LOOK_AT_HEIGHT, lookAtPoint.z);
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

/* Steps the camera foward by dT amount of time with spring-based movement. */
void Camera::step(float dT) {
   glm::vec3 newPos = position;
   glm::vec3 displacement;
   float dispLength = 0.0f;
   float springMag = 0.0f;
   float scalar = 0.0f;

   //Get our target position
   glm::vec3 camForward = getCamForwardVec();
   if (glm::length(position - lookAt) > MAX_DIST_FROM_DEER) {
      target += camForward * (dT / 100.0f) * speed;
   }

   if (glm::length(position - lookAt) < MIN_DIST_FROM_DEER) {
      target -=  camForward * (dT / 100.0f) * speed;
   }

   //Get the distance between the current position and our target
   displacement = newPos - target;
   dispLength = glm::length(displacement);

   //Only move if the distance is not super small
   if (dispLength >= 0.001) {
      //Find the magnitude of the spring for this distance
      springMag = springStrength * dispLength + dampConst * (1 / dispLength);

      //Calculate the scale of displacement based on the magnitude and distance
      scalar = (1.0f / dispLength) * springMag;

      //Apply the scale and move based on the scaled displacement
      displacement *= scalar;
      newPos.x -= displacement.x;
      newPos.y -= displacement.y;
      newPos.z -= displacement.z;
   }

   //Set the position
   position = newPos;

   //Bound the y so we don't go under the ground
   if (position.y < 1.0f) {
      position.y = 1.0f;
   }

   //Height offset to stay above the deer
   position.y += POSITION_HEIGHT;
}
