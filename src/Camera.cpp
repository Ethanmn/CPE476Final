/*
   Camera.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Camera.h"
#include <iostream>

#include "deer.h"

#include <glm/gtx/rotate_vector.hpp>

const float kSpringStrength(0.128f);  //Larger = stiffer spring
const float kDampConst(0.0065f);      //Larger = lighter mass (more spring motion),
const float kSpringSpeed(0.01f);      //Larger = faster spring movement
const float kCircleRadius(300.f);
const float kRotateSpeed(15.f / 1000.f);

const auto kUp = glm::vec3(0, 1, 0);

Camera::Camera(glm::vec3 pos, glm::vec3 look) :
   position(pos),
   lookAt(look),
   camera_target(pos),
   angle(0.f),
   spring_motion(kSpringStrength, kDampConst, kSpringSpeed)
{}


/* Retrieves the view matrix (glm::mat4) for shaders. */
glm::mat4 Camera::getViewMatrix() const {
   return glm::lookAt(position, lookAt, kUp);
}

/* Steps the camera foward by dT amount of time with spring-based movement. */
void Camera::step(float dT, const glm::vec3& target_pos, const glm::vec3& target_facing, Position relative_position) {
   // Update our lookat to look at the target
   lookAt = target_pos;
   //Get our target camera position from the position of the target
   switch (relative_position) {
      case Position::BEHIND:
         camera_target = target_pos - cameraDistanceToDeer * target_facing + glm::vec3(0, cameraHeightAboveDeer, 0);
         break;
      case Position::FRONT_RIGHT:
         camera_target = target_pos -
            cameraDistanceToDeer/2.f * glm::rotateY(target_facing, 110.f) +
            glm::vec3(0, cameraHeightAboveDeer / 3.f, 0);
         break;
      case Position::LEFT:
         camera_target = target_pos -
            cameraDistanceToDeer/1.5f * glm::rotateY(target_facing, -100.f) +
            glm::vec3(0, cameraHeightAboveDeer / 2.f, 0);
         break;
   }

   position = spring_motion.step(dT, camera_target, position);
}

void Camera::circle(float dT, const glm::vec3& target_pos) {
   lookAt = target_pos;
   angle += kRotateSpeed * dT;
   camera_target = position = glm::vec3(
         kCircleRadius * glm::cos(glm::radians(angle)),
         position.y,
         kCircleRadius * glm::sin(glm::radians(angle)));
}
