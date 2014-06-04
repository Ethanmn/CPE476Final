#include "deer.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>

#include "graphics/shader.h"
#include "graphics/location_maps.h"
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
const float kLeanFactor = 2.0f;
const float kTurnSpeed = 0.08f;

const float kStepTime = 300;

Deer::Deer(const Mesh& walk_mesh, const Mesh& eat_mesh, const glm::vec3& position) :
   draw_template_({
         ShaderType::TEXTURE,
         walk_mesh,
         Texture(TextureType::DEER, DIFFUSE_TEXTURE),
         boost::none,
         EffectSet({EffectType::CASTS_SHADOW, EffectType::CASTS_REFLECTION})
         }),
   eating_(false),
   walk_mesh_(walk_mesh),
   eat_mesh_(eat_mesh),
   up_(glm::vec3(0.0f, 1.0f, 0.0f)),
   position_(position),
   velocity_(0, 0, 0),
   last_facing_(0, 1),
   desired_lean_(0.0f),
   current_lean_(0.0f),
   walk_direction_(WalkDirection::NONE),
   bounding_rectangle_(xz(position_), glm::vec2(10.0f, 5.0f), 0.0f),
   step_timer_(0),
   is_jumping_(false),
   is_walking_(false),
   blocked(false)
      {}

glm::mat4 Deer::calculateModel() const {
   const glm::mat4 rotate(
         glm::lookAt(
            glm::vec3(0.0f),
            glm::vec3(last_facing_.x, 0.0f, -last_facing_.y),
            glm::vec3(0, 1, 0)));

   const glm::mat4 lean(
         glm::rotate(
            glm::mat4(),
            current_lean_ * kLeanFactor,
            glm::vec3(0, 0, 1)
            ));

   const glm::mat4 translate(
      glm::translate(
            glm::mat4(1.0f),
            position_));

   return glm::mat4(translate * rotate * lean);
}

Drawable Deer::drawable() const {
   return Drawable({draw_template_, std::vector<glm::mat4>({calculateModel()})});
} 

glm::vec3 Deer::predictPosition(units::MS dt, const glm::vec3& velocity) const {
   return position_ + velocity * static_cast<float>(dt);
}

BoundingRectangle Deer::getNextBoundingBox(units::MS dt) {
   auto velocity(predictVelocity(dt, acceleration()));
   auto facing(predictFacing(dt));
   auto tempPosition(predictPosition(dt, velocity));
   auto tempBoundingBox(bounding_rectangle_);
   tempBoundingBox.set_position(xz(tempPosition));
   tempBoundingBox.set_rotation(glm::degrees(std::atan2(-facing.y, facing.x)));
   return tempBoundingBox;
}

glm::vec3 Deer::acceleration() const {
   glm::vec3 acceleration(0.0f);
   { // If walking add in walk based on deer's forward.
      const glm::vec2 forward(last_facing_ / glm::length(last_facing_));
      if (walk_direction_ == WalkDirection::FORWARD) {
         acceleration = glm::vec3(forward.x, 0.0f, forward.y);
      }
   }
   return acceleration;
}

glm::vec3 Deer::predictVelocity(units::MS dt, const glm::vec3& acceleration) const {
   if (eating_) {
   }
   glm::vec3 velocity(velocity_);
   if (!has_acceleration()) {
      glm::vec2 xz_velocity(xz(velocity));
      xz_velocity -= xz_velocity * (kFriction * dt);
      if (glm::length(xz_velocity) < kSpeed / 4.0f) {
         xz_velocity = glm::vec2(0.0f);
      }
      velocity.x = xz_velocity.x;
      velocity.z = xz_velocity.y;
   } else {
      { // Accelerate velocity, capping at kSpeed.
         velocity += glm::normalize(acceleration) * (kAcceleration * dt);
         glm::vec2 xz_velocity(xz(velocity));
         if (glm::length(xz_velocity) > kSpeed) {
            xz_velocity = glm::normalize(xz_velocity) * kSpeed;
         }
         velocity.x = xz_velocity.x;
         velocity.z = xz_velocity.y;
      }
   }
   if (is_jumping_) {
      velocity.y -= kGravity * dt;
   }
   return velocity;
}

void Deer::eat() {
   eating_ = true;
   draw_template_.mesh = eat_mesh_;
}

glm::vec2 Deer::predictFacing(units::MS dt) const {
   float rotate = 0.f;
   if (turn_direction_ == TurnDirection::LEFT) {
      rotate -= kTurnSpeed * dt;
   } else if (turn_direction_ == TurnDirection::RIGHT) {
      rotate += kTurnSpeed * dt;
   }
   return glm::rotate(last_facing_, rotate);
}

void Deer::step(units::MS dt, const GroundPlane& ground_plane, SoundEngine& sound_engine) {
   current_lean_ += (desired_lean_ - current_lean_) * 0.1f;
   velocity_ = predictVelocity(dt, acceleration());
   if (eating_) {
      draw_template_.mesh.animation.step(dt);
      if (draw_template_.mesh.animation.is_finished()) {
         eating_ = false;
         draw_template_.mesh = walk_mesh_;
      }
      return;
   } else {
      // Turn
      const auto next_facing(predictFacing(dt));
      desired_lean_ = glm::orientedAngle(last_facing_, next_facing);
      last_facing_ = next_facing;
      if (desired_lean_ > 45.0f)
         desired_lean_ = 0.0f;
      if (desired_lean_ < -45.0f)
         desired_lean_ = 0.0f;
   }

   if (!has_acceleration()) {
      desired_lean_ = 0.0f;
   } else {
      draw_template_.mesh.animation.step(dt);
      if (draw_template_.mesh.animation.is_finished()) {
         draw_template_.mesh.animation.reset();
      }
   }
   if (is_jumping_) {
      const auto ground_height = ground_plane.heightAt(position_);
      if (position_.y + draw_template_.mesh.min.y < ground_height) {
         velocity_.y = 0.0f;
         position_.y = ground_height - draw_template_.mesh.min.y;
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
      position_.y = ground_plane.heightAt(position_) - draw_template_.mesh.min.y;
   } else {
      step_timer_ = 0;
      position_.y = ground_plane.heightAt(position_) - draw_template_.mesh.min.y;
   }

   if (!blocked) {
      position_ = predictPosition(dt, velocity_);

      bounding_rectangle_.set_position(xz(position_));
      bounding_rectangle_.set_rotation(glm::degrees(std::atan2(-last_facing_.y, last_facing_.x)));
   }
   blocked = false;
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

void Deer::turnLeft() {
   turn_direction_ = TurnDirection::LEFT;
}

void Deer::turnRight() {
   turn_direction_ = TurnDirection::RIGHT;
}

void Deer::stopTurning() {
   turn_direction_ = TurnDirection::NONE;
}

void Deer::jump() {
   if (!is_jumping_) {
      velocity_.y = kJumpSpeed;
      is_jumping_ = true;
   }
}

bool Deer::isMoving() {
   return velocity_.x != 0.0 && velocity_.z != 0.0;
}

glm::vec3 Deer::getPosition() const {
   return position_;
}

void Deer::block() {
   stopWalking();
   velocity_ = glm::vec3(0, 0, 0);
   blocked = true;
}

glm::vec3 Deer::getFacing() const {
   return glm::vec3(last_facing_.x, 0.0f, last_facing_.y);
}
