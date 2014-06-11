#include "deer.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>

#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/material.h"
#include "sound_engine.h"
#include "ground_plane.h"
#include "Flower.h"

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

Deer::Deer(const Mesh& walk_mesh, const Mesh& eat_mesh, const Mesh& sleep_mesh,
      const Mesh& pounce_mesh, const Mesh& dust, const glm::vec3& position) :
   draw_template_({
         ShaderType::DEFERRED,
         walk_mesh,
         Material(),
         Texture(TextureType::DEER, DIFFUSE_TEXTURE),
         boost::none,
         EffectSet({EffectType::CASTS_SHADOW, EffectType::CASTS_REFLECTION, EffectType::IS_GOD_RAY})
         }),
   eating_(false),
   sleeping_(false),
   walk_mesh_(walk_mesh),
   eat_mesh_(eat_mesh),
   sleep_mesh_(sleep_mesh),
   pounce_mesh_(pounce_mesh),
   spring_(false),
   model_state_({
         position,
         glm::vec3(0, 0, 0),
         glm::vec2(0, 1),
         0.0f,
         0.0f
         }),
   desired_lean_(0.0f),
   walk_direction_(WalkDirection::NONE),
   turn_direction_(TurnDirection::NONE),
   bounding_rectangle_(xz(model_state_.position),
         glm::vec2(draw_template_.mesh.max.z - draw_template_.mesh.min.z,
                   draw_template_.mesh.max.x - draw_template_.mesh.min.x), 0.0f),
   step_timer_(0),
   is_jumping_(false),
   is_walking_(false),
   blocked(false),
   pivot_(glm::translate(
            glm::mat4(),
            glm::vec3(0, 0, (draw_template_.mesh.max.z - draw_template_.mesh.min.z)) / 3.0f)),
   inverse_pivot_(glm::inverse(pivot_)),
   dust_system_front_(dust, TextureType::LEAF, position, 0),
   dust_system_back_(dust, TextureType::LEAF, position, 0),
   water_system_front_(dust, TextureType::RAIN, position, 0),
   water_system_back_(dust, TextureType::RAIN, position, 0)
      {}

void Deer::changeRock(units::MS dt, const GroundPlane& ground_plane) {
   glm::vec2 backFeet = back_feet_bounding_rectangle().getCenter();
   glm::vec2 frontFeet = front_feet_bounding_rectangle().getCenter();
   float heightFront = ground_plane.heightAt(glm::vec3(frontFeet.x, 0.0, frontFeet.y));
   float heightBack = ground_plane.heightAt(glm::vec3(backFeet.x, 0.0, backFeet.y));

   glm::vec3 feetVec = glm::vec3(frontFeet.x - backFeet.x, 
                                 heightFront - heightBack,
                                 frontFeet.y - frontFeet.y);
   float feetDot = glm::dot(glm::vec3(1,0,0), glm::normalize(feetVec));
   float feetAngle = glm::degrees(glm::acos(feetDot));
   desired_rock_ = feetAngle;
   
   if(desired_rock_ < model_state_.current_rock)
      model_state_.current_rock -= 0.00001 * dt;
   else if(desired_rock_ > model_state_.current_rock)
      model_state_.current_rock += 0.00001 * dt;
}

glm::mat4 Deer::calculateModel(const ModelState& model_state) const {
   const glm::mat4 rotate(
         glm::lookAt(
            glm::vec3(0.0f),
            glm::vec3(model_state.last_facing.x, 0.0f, -model_state.last_facing.y),
            glm::vec3(0, 1, 0)));

   const glm::mat4 lean(
         glm::rotate(
            glm::mat4(),
            model_state.current_lean * kLeanFactor,
            glm::vec3(0, 0, 1)
            ));

   const glm::mat4 translate(
      glm::translate(
            glm::mat4(1.0f),
            model_state.position));

   return glm::mat4(translate * inverse_pivot_ * rotate * pivot_ * lean);
}

Drawable Deer::drawable() const {
   const glm::mat4 rock(
         glm::rotate(
            glm::mat4(),
            model_state_.current_rock,
            glm::vec3(1, 0, 0)
            ));
   return Drawable({draw_template_, std::vector<DrawInstance>({calculateModel(model_state_) * rock})});
} 

glm::vec3 Deer::predictPosition(units::MS dt, const glm::vec3& velocity) const {
   return model_state_.position + velocity * static_cast<float>(dt);
}

BoundingRectangle Deer::getNextBoundingBox(units::MS dt) {
   auto velocity(predictVelocity(dt, acceleration()));
   auto facing(predictFacing(dt));
   auto tempPosition(predictPosition(dt, velocity));
   ModelState model_state({
         tempPosition,
         velocity,
         facing,
         0.0f // assume no lean.
         });
   return boundingRectangleFromModel(model_state);
}

glm::vec3 Deer::acceleration() const {
   glm::vec3 acceleration(0.0f);
   { // If walking add in walk based on deer's forward.
      const glm::vec2 forward(model_state_.last_facing / glm::length(model_state_.last_facing));
      if (walk_direction_ == WalkDirection::FORWARD) {
         acceleration = glm::vec3(forward.x, 0.0f, forward.y);
      } else if (walk_direction_ == WalkDirection::BACKWARD) {
         acceleration = glm::vec3(-forward.x, 0.0f, -forward.y);
      }
   }
   return acceleration;
}

glm::vec3 Deer::predictVelocity(units::MS dt, const glm::vec3& acceleration) const {
   glm::vec3 velocity(model_state_.velocity);
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

void Deer::pounce(const glm::vec2& pounce_target) {
   pounce_target_ = pounce_target;
   spring_ = false;
   draw_template_.mesh = pounce_mesh_;
}

void Deer::eat(Flower& flower) {
   if (!sleeping_) {
      eating_ = true;
      draw_template_.mesh = eat_mesh_;
      flower_ = flower;
   }
}

glm::vec2 Deer::predictFacing(units::MS dt) const {
   float rotate = 0.f;
   if (!sleeping_) {
      if (turn_direction_ == TurnDirection::LEFT) {
         rotate -= kTurnSpeed * dt;
      } else if (turn_direction_ == TurnDirection::RIGHT) {
         rotate += kTurnSpeed * dt;
      }
   }
   return glm::rotate(model_state_.last_facing, rotate);
}

void Deer::step(units::MS dt, const GroundPlane& ground_plane, SoundEngine& sound_engine) {
   changeRock(dt, ground_plane);
   if (pounce_target_ && spring_) walkForward();
   model_state_.current_lean += (desired_lean_ - model_state_.current_lean) * 0.1f;
   model_state_.velocity = predictVelocity(dt, acceleration());

   const auto offset = 60.f;
   if ((model_state_.position.x > GroundPlane::GROUND_SCALE / 2 - offset && model_state_.velocity.x > 0.f) ||
       (model_state_.position.x < -GroundPlane::GROUND_SCALE / 2 + offset && model_state_.velocity.x < 0.f) ||
       (model_state_.position.z > GroundPlane::GROUND_SCALE / 2 - offset && model_state_.velocity.z > 0.f) ||
       (model_state_.position.z < -GroundPlane::GROUND_SCALE / 2 + offset && model_state_.velocity.z < 0.f)) {
      blocked = true;
      if (!sleeping_)
         draw_template_.mesh = sleep_mesh_;
      sleeping_ = true;
      if (!draw_template_.mesh.animation.past_percentage(38. / 69.))
         draw_template_.mesh.animation.step(dt);
   } else if (sleeping_) {
      if ((model_state_.position.x > GroundPlane::GROUND_SCALE / 2 - offset) ||
          (model_state_.position.x < -GroundPlane::GROUND_SCALE / 2 + offset) ||
          (model_state_.position.z > GroundPlane::GROUND_SCALE / 2 - offset) ||
          (model_state_.position.z < -GroundPlane::GROUND_SCALE / 2 + offset)) {
         if (!draw_template_.mesh.animation.past_percentage(38. / 69.)) {
            draw_template_.mesh.animation.step(dt);
         }
      } else if (!draw_template_.mesh.animation.is_finished()) {
         blocked = true;
         draw_template_.mesh.animation.step(dt);
      } else {
         draw_template_.mesh = walk_mesh_;
         sleeping_ = false;
         blocked = false;
      }
   }

   if (eating_) {
      draw_template_.mesh.animation.step(dt);
      if (flower_ && draw_template_.mesh.animation.past_percentage(16. / 56.)) {
         flower_->eat(sound_engine);
         redFlowerTimer = 0.f;
         blueFlowerTimer = 0.f;
      }
      if (draw_template_.mesh.animation.is_finished()) {
         eating_ = false;
         draw_template_.mesh = walk_mesh_;
      }
      return;
   } else if (pounce_target_) {
      if (spring_) {
         model_state_.position = predictPosition(dt, model_state_.velocity);
         draw_template_.mesh.animation.step(dt);
         if (draw_template_.mesh.animation.past_percentage(32. / 40.)) {
            draw_template_.mesh.animation.set_percentage(16. / 40.);
         }
      } else {
         const auto target_facing = glm::normalize(*pounce_target_ -
               xz(glm::vec3(inverse_pivot_ * glm::vec4(model_state_.position, 1.f))));
         const auto angle = glm::orientedAngle(model_state_.last_facing, target_facing);
         if (glm::abs(angle) > 7.f) {
            const auto rotate_speed = 5.f / 16.f;
            const auto sign = angle > 1.f ? 1.f : -1.f;
            const auto abs_angle = std::min(dt * rotate_speed, glm::abs(angle));
            const auto rotate_angle = abs_angle * sign;
            model_state_.last_facing = glm::rotate(model_state_.last_facing, rotate_angle);
         } else {
            model_state_.last_facing = target_facing;
            draw_template_.mesh.animation.step(dt);
            if (draw_template_.mesh.animation.is_finished()) {
               spring_ = true;
               draw_template_.mesh = walk_mesh_;
            }
         }
      }
      bounding_rectangle_ = boundingRectangleFromModel(model_state_);
      return;
   } else {
      // Turn
      const auto next_facing(predictFacing(dt));
      desired_lean_ = glm::orientedAngle(model_state_.last_facing, next_facing);
      model_state_.last_facing = next_facing;
      if (desired_lean_ > 45.0f)
         desired_lean_ = 0.0f;
      if (desired_lean_ < -45.0f)
         desired_lean_ = 0.0f;
   }

   if (!has_acceleration()) {
      desired_lean_ = 0.0f;
   } else if (!sleeping_) {
      draw_template_.mesh.animation.step(dt);
      if (draw_template_.mesh.animation.past_percentage(32. / 40.)) {
         draw_template_.mesh.animation.set_percentage(16. / 40.);
      }
   }
   if (is_jumping_) {
      const auto ground_height = ground_plane.heightAt(model_state_.position);
      if (model_state_.position.y + draw_template_.mesh.min.y < ground_height) {
         model_state_.velocity.y = 0.0f;
         model_state_.position.y = ground_height - draw_template_.mesh.min.y;
         is_jumping_ = false;
         sound_engine.playSoundEffect(SoundEngine::SoundEffect::GRASS_LAND, false, model_state_.position);
      }
      step_timer_ = 0;
   } else if (isMoving()) {
      step_timer_ += dt;
      if (step_timer_ > kStepTime) {
         if (model_state_.position.y + draw_template_.mesh.min.y < 0.0f) {
            water_system_front_.add(glm::vec3(front_feet_bounding_rectangle().getCenter().x,
                                          model_state_.position.y + draw_template_.mesh.min.y,
                                          front_feet_bounding_rectangle().getCenter().y));
            water_system_back_.add(glm::vec3(front_feet_bounding_rectangle().getCenter().x,
                                          model_state_.position.y + draw_template_.mesh.min.y,
                                          front_feet_bounding_rectangle().getCenter().y));
         }
         else {
            dust_system_front_.add(glm::vec3(front_feet_bounding_rectangle().getCenter().x,
                                          model_state_.position.y + draw_template_.mesh.min.y,
                                          front_feet_bounding_rectangle().getCenter().y));
            dust_system_back_.add(glm::vec3(front_feet_bounding_rectangle().getCenter().x,
                                          model_state_.position.y + draw_template_.mesh.min.y,
                                          front_feet_bounding_rectangle().getCenter().y));
         }
         step_timer_ = 0;
         sound_engine.playRandomWalkSound();
      }
      model_state_.position.y = ground_plane.heightAt(model_state_.position) - draw_template_.mesh.min.y;
   } else {
      step_timer_ = 0;
      model_state_.position.y = ground_plane.heightAt(model_state_.position) - draw_template_.mesh.min.y;
   }

   if (!blocked) {
      model_state_.position = predictPosition(dt, model_state_.velocity);
   }
   dust_system_front_.step(dt);
   dust_system_back_.step(dt);
   water_system_front_.step(dt);
   water_system_back_.step(dt);

   blocked = false;

   bounding_rectangle_ = boundingRectangleFromModel(model_state_);
}

BoundingRectangle Deer::boundingRectangleFromModel(const ModelState& model_state) const {
   auto pos = calculateModel(model_state) * glm::vec4((draw_template_.mesh.max + draw_template_.mesh.min) / 2.f, 1.0f);
   BoundingRectangle rect(bounding_rectangle_);
   rect.set_position(glm::vec2(pos.x, pos.z));
   rect.set_rotation(glm::degrees(std::atan2(-model_state.last_facing.y, model_state.last_facing.x)));
   return rect;
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
      model_state_.velocity.y = kJumpSpeed;
      is_jumping_ = true;
   }
}

bool Deer::isMoving() {
   return model_state_.velocity.x != 0.0 && model_state_.velocity.z != 0.0;
}

glm::vec3 Deer::getPosition() const {
   return glm::vec3(bounding_rectangle().getCenter().x, model_state_.position.y, bounding_rectangle().getCenter().y);
}

void Deer::block() {
   stopWalking();
   model_state_.velocity = glm::vec3(0, 0, 0);
   blocked = true;
}

glm::vec3 Deer::getFacing() const {
   return glm::vec3(model_state_.last_facing.x, 0.0f, model_state_.last_facing.y);
}
