/* 
   Camera.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Camera.h"
#include <iostream>

const float PI = 3.14159265359;
const int MAX_ROTATE_VERT_UP =  80;
const int MAX_ROTATE_VERT_DOWN =  10;
const int PI_IN_DEGREES = 180;

Camera::Camera(glm::vec3 pos, glm::vec3 look) :
   position(pos),
   lookAt(look),
   up(glm::vec3(0, 1, 0)),
   target(pos),
   direction(0.0f, 0.0f, 0.0f),
   springStrength(0.1),
   dampConst(0.00065),
   angle(PI),
   vertAngle(asin(position.y / glm::length(look - pos)) * PI_IN_DEGREES / PI),
   movingFoward(false),
   movingBack(false),
   turningLeft(false),
   turningRight(false)
{
   //printf("Pos Y: %f\nVert Angle: %f\n", position.y, vertAngle);
}

/* 'Jumps' the camera to the specified position. */
void Camera::setPosition(const glm::vec3& endPosition) {
   position = endPosition;
}

/* Changes the point the camera is looking at. */
void Camera::setLookAt(const glm::vec3& lookAtPoint) {
   lookAt = lookAtPoint;
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

void Camera::turnLeft() {
   turningLeft = true;
}

void Camera::turnRight() {
   turningRight = true;
}

void Camera::moveFoward() {
   movingFoward = true;
}

void Camera::moveBack() {
   movingBack = true;
}

void Camera::step(float dT) {
   glm::vec3 newPos = position;
   glm::vec3 displacement;
   float dispLength = 0.0f;
   float springMag = 0.0f;
   float scalar = 0.0f;

   if (turningLeft) {
      angle -= 90.0f * dT / 100.0f;
   }
   
   if (turningRight) {
      angle += 90.0f * dT / 100.0f;
   }

   if (vertAngle > 15.0f) {
      vertAngle -= dT / 100.0f;
   }
   else if (vertAngle < 5.0f) {
      vertAngle += dT / 100.0f;
   }

   if (glm::length(position - lookAt) > 25.0f) {
      target += getCamForwardVec() * (dT / 100.0f) * 5.0f;
   }

   //printf("Length: %f\n", glm::length(position - lookAt)); 

   if (glm::length(position - lookAt) < 15.0f) {
      target -=  getCamForwardVec() * (dT / 100.0f) * 5.0f;
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

   //printf("Position: (%f, %f, %f)\nTarget: (%f, %f, %f)\nDisplacement: (%f, %f, %f)\nDisplacement Length: %f\nSpring Magnitude: %f\nScalar: %f\n", position.x, position.y, position.z, target.x, target.y, target.z, displacement.x, displacement.y, displacement.z, dispLength, springMag, scalar);

   position = newPos;
   rotateCamera(angle);

   turningLeft = false;
   turningRight = false;
   movingFoward = false;
   movingBack = false;

   if (position.y < 1.0f) {
      position.y = 0.0f;
   }

   //printf("Pos Y: %f\nVert Angle: %f\n", position.y, vertAngle);

   //printCamera();
}

void Camera::rotateCamera(float angle) {
   position = glm::vec3(cos(vertAngle) * cos(angle) + position.x, sin(vertAngle) + position.y, cos(vertAngle) * cos(PI / 2 - angle) + position.z);
}

void Camera::printCamera() {
   printf("Position: (%f, %f, %f)\nDirection: (%f, %f, %f)\nAngle: %f\n", position.x, position.y, position.z, direction.x, direction.y, direction.z, angle);
}
