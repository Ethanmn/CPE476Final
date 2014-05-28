#include "Bush.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

Bush::Bush(const Mesh& mesh, const glm::vec3& position, const GroundPlane& ground, 
      float scale, units::MS rustle_time) :
   rotate_(0.0f),
   elapsed_time_(0),
   rustle_time_(rustle_time),
   kMaxRustleTime(rustle_time),
   bounding_rectangle_(BoundingRectangle(
            glm::vec2(position.x, position.z),
            glm::vec2(8.0f, 8.0f),
            0.0f)),
   translate_scale_(
         glm::translate(glm::mat4(), glm::vec3(position.x, ground.heightAt(position) - mesh.min.y, position.z)) *
         glm::scale(glm::mat4(), glm::vec3(scale)))
{}

void Bush::step(units::MS dt) {
   if (rustle_time_ < kMaxRustleTime) {
      rustle_time_ += dt;
      elapsed_time_ += dt;
      rotate_ = 10 * glm::sin(elapsed_time_ / 60.0f);
   }
}

glm::mat4 Bush::calculateModel() const {
   const glm::mat4 rotate(glm::rotate(
            glm::rotate(
               glm::mat4(),
               rotate_,
               glm::vec3(0, 1, 0)
               ),
            -90.0f,
            glm::vec3(1, 0, 0)));
   return translate_scale_ * rotate;
} 

void Bush::rustle(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::RUSTLE,
         false,
         glm::vec3());
   rustle_time_ = 0;
}

bool Bush::isBlocker() {
   return false;
}

void Bush::performObjectHit(SoundEngine& sound_engine) {
   rustle(sound_engine);
}
