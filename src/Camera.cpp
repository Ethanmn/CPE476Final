/* 
   Camera.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Camera.h"

const float PI = 3.14159265359;
const int MAX_ROTATE_VERT =  80;
const int PI_IN_DEGREES = 180;

Camera::Camera(glm::vec3 pos, glm::vec3 look) {
   position = pos;
   lookAt = look;
   up = glm::vec3(0, 1, 0);
   phi = 0;
   theta = -PI / 2;
}

/* 
   'Jumps' the camera to the specified position.
   There is no transition.
*/
void Camera::setPosition(glm::vec3& endPosition) {
   position = endPosition;
   updateLookAt();
}

/* Changes what the camera is looking at to the specified point. */
void Camera::setLookAt(glm::vec3& lookAtPoint) {
   lookAt = lookAtPoint;
}

/* Moves the camera one step toward the specified position at the given speed. */
void Camera::moveCameraToPoint(glm::vec3& endPosition, float speed, units::MS timeDif) {
   position += (endPosition - position) / glm::length(endPosition - position) * speed * (float)timeDif;
   updateLookAt();
}

/* Moves the camera one step in the given direction at the given speed. */
void Camera::moveCameraInDirection(glm::vec3& direction, float speed, units::MS timeDif) {
   position += direction / glm::length(direction) * speed * (float)timeDif;
   updateLookAt();
}

/*
   Rotates the camera based on a mouse drag.
   Parameters should be the starting mouse point,
   the ending mouse point, the width of the window,
   and the height of the window.
*/
void Camera::rotateWithDrag(glm::vec3& startPoint, glm::vec3& endPoint, int width, int height) {
   theta += (endPoint.x - startPoint.x) * PI / width;
   phi += (endPoint.y - startPoint.y) * PI / height;

   if (phi > MAX_ROTATE_VERT * PI / PI_IN_DEGREES) {
     phi = MAX_ROTATE_VERT * PI / PI_IN_DEGREES;
   }
   else if (phi < -MAX_ROTATE_VERT * PI / PI_IN_DEGREES) {
      phi = -MAX_ROTATE_VERT * PI / PI_IN_DEGREES;
   }

   updateLookAt();
}

/* Retrieves the current position of the camera. */
glm::vec3 Camera::getPosition() {
   return position;
}

/* Retrieves the vector pointing straight in front of the camera. */
glm::vec3 Camera::getCamForwardVec() {
   return (lookAt - position) / glm::length(lookAt - position);
}

/* Retrieves the vector pointing to the left of the camera. */
glm::vec3 Camera::getCamLeftVec() {
   glm::vec3 w = getCamForwardVec();
   return glm::cross(up, w) / glm::length(glm::cross(up, w));
}

/* Retrieves the view matrix (glm::mat4) for shaders. */
glm::mat4 Camera::getViewMatrix() {
   return glm::lookAt(position, lookAt, up);
}

/* 
   A private function to update our lookAt point.
   This should be called whenever the camera is rotated or translated.
*/
void Camera::updateLookAt() {
   lookAt = glm::vec3(cos(phi) * cos(theta) + position.x, sin(phi), cos(phi) * cos(PI / 2 - theta) + position.z);
}
