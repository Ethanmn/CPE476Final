/*
   Camera.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Camera.h"
#include <iostream>

#include "deer.h"

#include <glm/gtx/rotate_vector.hpp>

//Height offsets
const float kLookAtHeight = 12.0f;

const float kSpringStrength(0.128f);  //Larger = stiffer spring
const float kDampConst(0.0065f);      //Larger = lighter mass (more spring motion),
const float kSpringSpeed(0.01f);      //Larger = faster spring movement
const float kDistanceFromDeer(20.0f);
const float kCircleRadius(300.f);
const float kRotateSpeed(15.f / 1000.f);

const auto kUp = glm::vec3(0, 1, 0);

Camera::Camera(glm::vec3 pos, glm::vec3 look) :
   position(pos),
   lookAt(look),
   camera_target(pos),
   angle(0.f)
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
         camera_target = target_pos - kDistanceFromDeer * target_facing + glm::vec3(0, kLookAtHeight, 0);
         break;
      case Position::FRONT_RIGHT:
         camera_target = target_pos -
            kDistanceFromDeer/2.f * glm::rotateY(target_facing, 110.f) +
            glm::vec3(0, kLookAtHeight / 3.f, 0);
         break;
      case Position::LEFT:
         camera_target = target_pos -
            kDistanceFromDeer/1.5f * glm::rotateY(target_facing, -100.f) +
            glm::vec3(0, kLookAtHeight / 2.f, 0);
         break;
   }

   //Get the distance between the current position and our target
   const auto displacement = position - camera_target;
   const auto dispLength = glm::length(displacement);

   //Only move if the distance is not super small
   if (dispLength >= 0.09f) {
      //Find the magnitude of the spring for this distance
      const auto springMag = kSpringStrength * dispLength + kDampConst * (1 / dispLength);

      //Calculate the scale of displacement based on the magnitude and distance
      const auto scalar = (1.0f / dispLength) * springMag;

      //Apply the scale and move based on the scaled displacement
      position -= dT * kSpringSpeed * displacement * scalar;
   } else {
      position = camera_target;
   }
}

void Camera::circle(float dT, const glm::vec3& target_pos) {
   lookAt = target_pos;
   angle += kRotateSpeed * dT;
   camera_target = position = glm::vec3(kCircleRadius*glm::cos(glm::radians(angle)), position.y, kCircleRadius*glm::sin(glm::radians(angle)));
}
