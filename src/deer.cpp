#include "deer.h"

#include "graphics/shader.h"
#include "graphics/uniform_location_map.h"

const float kSpeed = 0.005f;
const float kFriction = 0.005f;
const float kAcceleration = 0.00007f;

void Deer::draw(Shader& shader, const UniformLocationMap& model_locations) const {
   const glm::mat4 model_matrix(glm::translate(glm::mat4(1.0f), position_));
   shader.sendUniform(model_locations, model_matrix);
   shader.drawMesh(mesh_);
}

void Deer::step(units::MS dt, const Camera& camera) {
   if (walk_direction_ == WalkType::NONE && strafe_direction_ == StrafeType::NONE) {
      velocity_ -= velocity_ * (kFriction * dt);
      if (glm::length(velocity_) < kSpeed / 4.0f) {
         velocity_ = glm::vec3(0.0f);
      }
   } else {
      glm::vec3 acceleration(0.0f);
      if (walk_direction_ == WalkType::FORWARD) {
         acceleration = camera.getCamForwardVec();
      } else if (walk_direction_ == WalkType::BACKWARD) {
         acceleration = -camera.getCamForwardVec();
      }
      if (strafe_direction_ == StrafeType::LEFT) {
         acceleration += camera.getCamLeftVec();
      } else if (strafe_direction_ == StrafeType::RIGHT) {
         acceleration -= camera.getCamLeftVec();
      }
      velocity_ += glm::normalize(acceleration) * (kAcceleration * dt);
      if (glm::length(velocity_) > kSpeed) {
         velocity_ = glm::normalize(velocity_) * kSpeed;
      }
   }
   position_ += velocity_ * static_cast<float>(dt);
}

void Deer::walkForward() {
   walk_direction_ = WalkType::FORWARD;
}
void Deer::walkBackward() {
   walk_direction_ = WalkType::BACKWARD;
}
void Deer::stopWalking() {
   walk_direction_ = WalkType::NONE;
}

void Deer::strafeLeft() {
   strafe_direction_ = StrafeType::LEFT;
}
void Deer::strafeRight() {
   strafe_direction_ = StrafeType::RIGHT;
}
void Deer::stopStrafing() {
   strafe_direction_ = StrafeType::NONE;
}
