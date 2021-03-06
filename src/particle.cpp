#include "particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"

void Particle::step(units::MS dt) {
   life_time_ += dt;
   velocity_ += acceleration_ * (float)dt;
   position_ += velocity_ * (float)dt;
}

glm::mat4 Particle::calculateModel() const {
   const glm::mat4 translate(
      glm::translate(
         glm::mat4(),
         position_
      )
   );

   const glm::mat4 scale(
      glm::scale(
         glm::mat4(),
         glm::vec3(scale_)
      )
   );
      
   const glm::mat4 rotate(
      glm::rotate(
         glm::mat4(),
         rotate_,
         glm::vec3(0, 1.0f, 0)
      )
   );
      
   return glm::mat4(translate * scale * rotate);
}

void Particle::setAccel(float x, float y, float z) {
   acceleration_ = glm::vec3(x, y, z);
}

void Particle::setVel(float x, float y, float z) {
   velocity_ = glm::vec3(x, y, z);
}

void Particle::setPos(float x, float y, float z) {
   position_ = glm::vec3(x, y, z);
}

void Particle::setRot(float angle) {
   rotate_ = angle;
}

glm::vec3 Particle::getPos() {
   return position_;
}

glm::vec3 Particle::getVel() {
   return velocity_;
}

glm::vec3 Particle::getAccel() {
   return acceleration_;
}

units::MS Particle::getLife() {
   return life_time_;
}

float Particle::getRot() {
   return rotate_;
}
