#include "Bush.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

Bush::Bush(const Mesh& mesh, const glm::vec3& position, float angleOffset, const GroundPlane& ground, 
      float scale, units::MS rustle_time, const Mesh& butterfly) :
   rotate_(0.0f),
   elapsed_time_(0),
   rustle_time_(rustle_time),
   kMaxRustleTime(rustle_time),
   bounding_rectangle_(BoundingRectangle(
            glm::vec2(position.x, position.z),
            glm::vec2(8.0f * scale, 8.0f * scale),
            0.0f)),
   translate_scale_(
         glm::translate(glm::mat4(), glm::vec3(position.x, ground.heightAt(position) - mesh.min.y, position.z)) *
         glm::scale(glm::mat4(), glm::vec3(scale))),
   default_model_(
      translate_scale_ *
      glm::rotate(
         glm::rotate(
            glm::mat4(),
            angleOffset,
            glm::vec3(0, 1, 0)
         ),
         -90.0f,
         glm::vec3(1, 0, 0)
      )
   ),
   butterfly_system_(butterfly, TextureType::BUTTERFLY_BLUE, position, 0)
{
   if (rand() % 2)
      hasButterflies_ = false;
   else {
      hasButterflies_ = true;
      int color = rand() % 2;
      if (color == 0)
         butterfly_system_ = ButterflySystem(butterfly, TextureType::BUTTERFLY_RED, glm::vec3(position.x, position.y + 20.0f, position.y), 0);
      else if (color == 1)
         butterfly_system_ = ButterflySystem(butterfly, TextureType::BUTTERFLY_PINK, glm::vec3(position.x, position.y + 20.0f, position.y), 0);
      else
         butterfly_system_ = ButterflySystem(butterfly, TextureType::BUTTERFLY_BLUE, glm::vec3(position.x, position.y + 20.0f, position.y), 0);
   }
}

void Bush::step(units::MS dt) {
   if (rustle_time_ < kMaxRustleTime) {
      rustle_time_ += dt;
      elapsed_time_ += dt;
      rotate_ = 10 * glm::sin(elapsed_time_ / 60.0f);
   }
   butterfly_system_.step(dt);
}

glm::mat4 Bush::calculateModel() const {
   if (rustle_time_ >= kMaxRustleTime) {
      return default_model_;
   }
   const glm::mat4 rotate(
      glm::rotate(
         glm::mat4(),
         rotate_,
         glm::vec3(0, 0, 1.0f)
      )
   );
   return default_model_ * rotate;
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

   if (hasButterflies_) {
      butterfly_system_.add();
      hasButterflies_ = false;
   }
}
