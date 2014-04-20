#include "deer.h"

#include "graphics/shader.h"
#include "graphics/uniform_location_map.h"

const float kSpeed = 0.005f;

void Deer::draw(Shader& shader, const UniformLocationMap& model_locations) const {
   const glm::mat4 model_matrix(glm::translate(glm::mat4(1.0f), position_));
   shader.sendUniform(model_locations, model_matrix);
   shader.drawMesh(mesh_);
}

void Deer::step(units::MS dt, const Camera& camera) {
   if (walk_direction_ == WalkType::NONE && strafe_direction_ == StrafeType::NONE)
      return;
   glm::vec3 velocity(0.0f);
   if (walk_direction_ == WalkType::FORWARD)
      velocity = camera.getCamForwardVec();
   else if (walk_direction_ == WalkType::BACKWARD)
      velocity = -camera.getCamForwardVec();
   if (strafe_direction_ == StrafeType::LEFT)
      velocity += camera.getCamLeftVec();
   else if (strafe_direction_ == StrafeType::RIGHT)
      velocity -= camera.getCamLeftVec();
   velocity = glm::normalize(velocity) * (kSpeed * dt);
   position_ += velocity;
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
