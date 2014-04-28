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
const float ROTATION_SENSITIVITY = 2; //Smaller number -> less sensitive

Camera::Camera(glm::vec3 pos, glm::vec3 look) {
   position = pos;
   lookAt = look;
   up = glm::vec3(0, 1, 0);
   phi = PI / 4;
   theta = -PI / 2;
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

/*
   Rotates the camera around the current lookAt point, changing the camera's position.
   Parameters should be the starting mouse point,
   the ending mouse point, tthe width of the window,
   and the height of the window.
*/
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
   theta += (diffX * PI / (float)width) * ROTATION_SENSITIVITY;
   phi += (diffY * PI / (float)height) * ROTATION_SENSITIVITY;

   if (phi > MAX_ROTATE_VERT_UP * PI / PI_IN_DEGREES) {
     phi = MAX_ROTATE_VERT_UP * PI / PI_IN_DEGREES;
   }
   else if (phi < -MAX_ROTATE_VERT_DOWN * PI / PI_IN_DEGREES) {
      phi = -MAX_ROTATE_VERT_DOWN * PI / PI_IN_DEGREES;
   }
}
