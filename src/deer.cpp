#include "deer.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>

#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/shader_setup.h"
#include "graphics/material.h"
#include "sound_engine.h"
#include "ground_plane.h"

namespace {
   glm::vec2 xz(const glm::vec3& vec) {
      return std::move(glm::vec2(vec.x, vec.z));
   }
}

const float kSpeed = 0.025f;
const float kFriction = 0.005f;
const float kGravity = 0.00006f;
const float kAcceleration = 0.00007f;
const float kJumpSpeed = 0.015f;

const float kStepTime = 300;

Deer::Deer(const Mesh& mesh, const glm::vec3& position) :
   mesh_(mesh),
   texture_(texture_path(Textures::DEER)),
   position_(position),
   velocity_(0, 0, 0),
   last_facing_(0, 1),
   desired_lean_(0.0f),
   current_lean_(0.0f),
   walk_direction_(WalkDirection::NONE),
   strafe_direction_(StrafeDirection::NONE),
   bounding_rectangle_(xz(position_), glm::vec2(10.0f, 5.0f), 0.0f),
   step_timer_(0),
   is_jumping_(false),
   is_walking_(false),
   is_strafing_(false)
      {}

void Deer::draw(Shader& shader, const UniformLocationMap& uniform_locations,
                const glm::mat4& viewMatrix) const {
   const glm::mat4 rotate(
         glm::lookAt(
            glm::vec3(0.0f),
            glm::vec3(last_facing_.x, 0.0f, -last_facing_.y),
            glm::vec3(0, 1, 0)));

   const glm::mat4 lean(
         glm::rotate(
            glm::mat4(),
            current_lean_ * 5.0f,
            glm::vec3(0, 0, 1)
            ));

   const glm::mat4 translate(
      glm::translate(
            glm::mat4(1.0f),
            position_));
   const glm::mat4 transform(translate * rotate * lean);

   setupTextureShader(shader, uniform_locations, texture_);

   setupModelView(shader, uniform_locations, transform, viewMatrix, true);
   shader.sendUniform(Uniform::HAS_BONES, uniform_locations, 1);
   shader.sendUniform(Uniform::BONES, uniform_locations,
         mesh_.animation.calculateBoneTransformations(mesh_.bone_array));
   shader.drawMesh(mesh_);

   shader.sendUniform(Uniform::HAS_BONES, uniform_locations, 0);
   texture_.disable();
}

void Deer::step(units::MS dt, const Camera& camera, const GroundPlane& ground_plane, SoundEngine& sound_engine) {
   current_lean_ += (desired_lean_ - current_lean_) * 0.1f;
   if (walk_direction_ == WalkDirection::NONE && strafe_direction_ == StrafeDirection::NONE) {
      glm::vec2 xz_velocity(xz(velocity_));
      xz_velocity -= xz_velocity * (kFriction * dt);
      if (glm::length(xz_velocity) < kSpeed / 4.0f) {
         xz_velocity = glm::vec2(0.0f);
      }
      velocity_.x = xz_velocity.x;
      velocity_.z = xz_velocity.y;
   } else {
      mesh_.animation.step(dt);
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
         {
            const auto old_facing = last_facing_;
            last_facing_ = glm::normalize(glm::vec2(
                     velocity_.x,
                     velocity_.z));
            desired_lean_ = glm::orientedAngle(old_facing, last_facing_);
            if (desired_lean_ > 45.0f)
               desired_lean_ = 45.0f;
            if (desired_lean_ < -45.0f)
               desired_lean_ = -45.0f;
         }
      }
   }
   if (is_jumping_) {
      velocity_.y -= kGravity * dt;
      const auto ground_height = ground_plane.heightAt(position_);
      if (position_.y + mesh_.min.y < ground_height) {
         velocity_.y = 0.0f;
         position_.y = ground_height - mesh_.min.y;
         is_jumping_ = false;
         sound_engine.playSoundEffect(SoundEngine::SoundEffect::GRASS_LAND, false, position_);
      }
      step_timer_ = 0;
   } else if (isMoving()) {
      step_timer_ += dt;
      if (step_timer_ > kStepTime) {
         step_timer_ = 0;
         sound_engine.playRandomWalkSound();
      }
   } else {
      step_timer_ = 0;
      position_.y = ground_plane.heightAt(position_) - mesh_.min.y;
   }

   position_ += velocity_ * static_cast<float>(dt);

   bounding_rectangle_.set_position(xz(position_));
   bounding_rectangle_.set_rotation(glm::degrees(std::atan2(-last_facing_.y, last_facing_.x)));
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
   return velocity_.x > 0 || velocity_.z > 0 || velocity_.x < 0 || velocity_.z < 0;
}

glm::vec3 Deer::getPosition() const {
   return position_;
}

void Deer::shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir) {
   const glm::mat4 rotate(
         glm::lookAt(
            glm::vec3(0.0f),
            glm::vec3(last_facing_.x, last_facing_.y, -last_facing_.z),
            glm::vec3(0, 1, 0)));
   const glm::mat4 translate(
      glm::translate(
            glm::mat4(1.0f),
            position_));
   glm::mat4 model_matrix = translate * rotate;
   setupShadowShader(shader, uniform_locations, sunDir, getPosition(), model_matrix);
   shader.drawMesh(mesh_);
}

glm::vec3 Deer::getFacing() const {
   return glm::vec3(last_facing_.x, 0.0f, last_facing_.y);
}
