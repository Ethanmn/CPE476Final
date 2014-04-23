#include "deer.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>

#include "graphics/shader.h"
#include "graphics/location_maps.h"

namespace {
   glm::vec2 xz(const glm::vec3& vec) {
      return std::move(glm::vec2(vec.x, vec.z));
   }
}

const float kSpeed = 0.010f;
const float kFriction = 0.005f;
const float kGravity = 0.0005f;
const float kAcceleration = 0.00007f;
const float kJumpSpeed = 0.05f;

Deer::Deer(const Mesh& mesh, const glm::vec3& position) :
   mesh_(mesh),
   position_(position),
   velocity_(0, 0, 0),
   last_facing_(0, 0, 1),
   walk_direction_(WalkDirection::NONE),
   strafe_direction_(StrafeDirection::NONE),
   bounding_rectangle_(xz(position_), glm::vec2(10.0f, 5.0f), 0.0f)
      {}

void Deer::draw(Shader& shader, const UniformLocationMap& uniform_locations) const {
   const glm::mat4 rotate(
         glm::lookAt(
            glm::vec3(0.0f),
            glm::vec3(last_facing_.x, last_facing_.y, -last_facing_.z),
            glm::vec3(0, 1, 0)));
   const glm::mat4 translate(
      glm::translate(
            glm::mat4(1.0f),
            position_));
   const glm::mat4 model_matrix(translate * rotate);
   shader.sendUniform(Uniform::MODEL, uniform_locations, model_matrix);
   shader.sendUniform(Uniform::COLOR, uniform_locations, glm::vec4(0, 0, 1, 0.5f));
   shader.drawMesh(mesh_);
   bounding_rectangle_.draw(uniform_locations, shader, 0.0f);
}

void Deer::step(units::MS dt, const Camera& camera) {
   if (walk_direction_ == WalkDirection::NONE && strafe_direction_ == StrafeDirection::NONE) {
      glm::vec2 xz_velocity(xz(velocity_));
      xz_velocity -= xz_velocity * (kFriction * dt);
      if (glm::length(xz_velocity) < kSpeed / 4.0f) {
         xz_velocity = glm::vec2(0.0f);
      }
      velocity_.x = xz_velocity.x;
      velocity_.z = xz_velocity.y;
   } else {
      glm::vec3 acceleration(0.0f);
      { // If walking add in walk based on camera's forward.
         const glm::vec2 forward(xz(camera.getCamForwardVec()));
         if (walk_direction_ == WalkDirection::FORWARD) {
            acceleration = glm::vec3(forward.x, 0.0f, forward.y);
         } else if (walk_direction_ == WalkDirection::BACKWARD) {
            acceleration = -glm::vec3(forward.x, 0.0f, forward.y);
         }
      }

      { // Add in strafe from camera, if strafing.
         const glm::vec3 left(camera.getCamLeftVec());
         if (strafe_direction_ == StrafeDirection::LEFT) {
            acceleration += glm::vec3(left.x, 0.0f, left.z);
         } else if (strafe_direction_ == StrafeDirection::RIGHT) {
            acceleration -= glm::vec3(left.x, 0.0f, left.z);
         }
      }
      { // Accelerate velocity, capping at kSpeed.
         velocity_ += glm::normalize(acceleration) * (kAcceleration * dt);
         glm::vec2 xz_velocity(xz(velocity_));
         if (glm::length(xz_velocity) > kSpeed) {
            xz_velocity = glm::normalize(xz_velocity) * kSpeed;
         }
         velocity_.x = xz_velocity.x;
         velocity_.z = xz_velocity.y;
         last_facing_ = glm::normalize(glm::vec3(
                  velocity_.x,
                  0.0f,
                  velocity_.z));
      }
   }
   if (is_jumping_) {
      velocity_.y -= kGravity * dt;
      if (velocity_.y < 0) {
         velocity_.y = 0.0f;
         position_.y = 0.0f;
         is_jumping_ = false;
      }
   }

   position_ += velocity_ * static_cast<float>(dt);

   bounding_rectangle_.set_position(xz(position_));
   const auto xz_last_facing(xz(last_facing_));
   bounding_rectangle_.set_rotation(glm::degrees(std::atan2(-xz_last_facing.y, xz_last_facing.x)));
}

void Deer::walkForward() {
   walk_direction_ = WalkDirection::FORWARD;
   is_walking_ = true;
}
void Deer::walkBackward() {
   walk_direction_ = WalkDirection::BACKWARD;
   is_walking_ = true;
}
void Deer::stopWalking() {
   walk_direction_ = WalkDirection::NONE;
   is_walking_ = false;
}

void Deer::strafeLeft() {
   strafe_direction_ = StrafeDirection::LEFT;
   is_strafing_ = true;
}
void Deer::strafeRight() {
   strafe_direction_ = StrafeDirection::RIGHT;
   is_strafing_ = true;
}
void Deer::stopStrafing() {
   strafe_direction_ = StrafeDirection::NONE;
   is_strafing_ = false;
}

void Deer::jump() {
   if (!is_jumping_) {
      velocity_.y = kJumpSpeed;
      is_jumping_ = true;
   }
}

bool Deer::isMoving() {
   return is_walking_ || is_strafing_;
}

glm::vec3 Deer::getPosition() {
   return position_;
}
