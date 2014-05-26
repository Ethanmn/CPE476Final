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
const float ROTATION_SENSITIVITY = 1; //Smaller number -> less sensitive

const float TURN_SPEED = 4.0f;
const float FRICTION = 2.0f;

Camera::Camera(glm::vec3 pos, glm::vec3 look) :
   position(pos),
   lookAt(look),
   up(glm::vec3(0, 1, 0)),
   target(pos),
   direction(0.0f, 0.0f, 0.0f),
   phi(PI / 4),
   theta(-PI / 2),
   dTheta(0.0f),
   dPhi(0.0f),
   springStrength(0.3),
   dampConst(0.00065),
   angle (0.0f),
   movingFoward(false),
   movingBack(false),
   turningLeft(false),
   turningRight(false)
{
   //updatePosition(glm::length(position - lookAt));
}

/* 'Jumps' the camera to the specified position. */
void Camera::setPosition(const glm::vec3& endPosition) {
   position = endPosition;
   updateLookAt();
}

/* Changes the point the camera is looking at. */
void Camera::setLookAt(const glm::vec3& lookAtPoint) {
   lookAt = lookAtPoint;
}

/* Moves the camera one step toward the specified position at the given speed. */
void Camera::moveCameraToPoint(const glm::vec3& endPosition, float speed, units::MS timeDif) {
   position += (endPosition - position) / glm::length(endPosition - position) * speed * (float)timeDif;
   updateLookAt();
}

/* Moves the camera one step in the given direction at the given speed. */
void Camera::moveCameraInDirection(const glm::vec3& direction, float speed, units::MS timeDif) {
   position += direction / glm::length(direction) * speed * (float)timeDif;
   updateLookAt();
}

/*
   Rotates the camera based on a mouse drag, changing the camera's lookAt point.
   Parameters should be the starting mouse point,
   the ending mouse point, the width of the window,
   and the height of the window.
*/
void Camera::rotateLookAtWithDrag(const glm::vec2& startPoint, const glm::vec2& endPoint, int width, int height) {
   changeRotationAngles(endPoint.x - startPoint.x, endPoint.y - startPoint.y, width, height);
   updateLookAt();
}

/*
   Rotates the camera around the current lookAt point, changing the camera's position.
   Parameters should be the starting mouse point,
   the ending mouse point, tthe width of the window,
   and the height of the window.
*/
void Camera::rotatePositionWithDrag(const glm::vec2& startPoint, const glm::vec2& endPoint, int width, int height) {
   changeRotationAngles(endPoint.x - startPoint.x, endPoint.y - startPoint.y, width, height);
   updatePosition(glm::length(position - lookAt));
}

void Camera::rotatePositionWithDrag(float diffX, float diffY, int width, int height) {
   changeRotationAngles(diffX, diffY, width, height);
   updatePosition(glm::length(position - lookAt));
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

/* A private function to update our lookAt point. */
void Camera::updateLookAt() {
   lookAt = glm::vec3(cos(phi) * cos(theta) + position.x, sin(phi), cos(phi) * cos(PI / 2 - theta) + position.z);
}

/* A private function to update the position based on the current rotation angle and lookAt point. */
void Camera::updatePosition(float radius) {
   position = glm::vec3(radius * cos(phi) * cos(theta) + lookAt.x, radius * sin(phi), radius * cos(phi) * cos(PI / 2 - theta) + lookAt.z);
}

/* A private function called to change the current rotation angles */
void Camera::changeRotationAngles(float diffX, float diffY, int width, int height) {

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
      angle -= 90.0f * dT;
   }
   
   if (turningRight) {
      angle += 90.0f * dT;
   }

   if (glm::length(position - lookAt) > 12.0f) {
      target += getCamForwardVec() * (dT / 100.0f);// * 5.0f;
   }

   //printf("Length: %f\n", glm::length(position - lookAt)); 

   if (movingBack) {
      target -=  getCamForwardVec() * (dT / 100.0f);// * 5.0f;
   }

   displacement = newPos - target;
   dispLength = glm::length(displacement);

   if (dispLength >= 0.001) {
       springMag = springStrength * dispLength + dampConst * (1/dispLength);
      scalar = (1.0f / dispLength) * springMag * dT / 100.0f;
      displacement *= scalar;
      newPos.x -= displacement.x;
      newPos.z -= displacement.z;
   }

   //printf("Position: (%f, %f, %f)\nTarget: (%f, %f, %f)\nDisplacement: (%f, %f, %f)\nDisplacement Length: %f\nSpring Magnitude: %f\nScalar: %f\n", position.x, position.y, position.z, target.x, target.y, target.z, displacement.x, displacement.y, displacement.z, dispLength, springMag, scalar);

   position = newPos;
   rotateCamera(angle);

   turningLeft = false;
   turningRight = false;
   movingFoward = false;
   movingBack = false;

   //printCamera();
}

void Camera::rotateCamera(float angle) {
   position = glm::vec3(cos(0.0f) * cos(angle) + position.x, position.y, cos(0.0f) * cos(PI / 2 - angle) + position.z);
}

void Camera::printCamera() {
   printf("Position: (%f, %f, %f)\nDirection: (%f, %f, %f)\nAngle: %f\n", position.x, position.y, position.z, direction.x, direction.y, direction.z, angle);
}
