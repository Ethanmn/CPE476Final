#include "Bush.h"
#include "graphics/shader_setup.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

void Bush::step(units::MS dt) {
   if (rustle_time_ < kMaxRustleTime) {
      rustle_time_ += dt;
      elapsed_time_ += dt;
      rotate_ = 10 * glm::sin(elapsed_time_ / 60.0f);
   }
}

glm::mat4 Bush::calculateModel() const {
   const glm::mat4 translate(glm::translate(
            glm::mat4(),
            position_));
   const glm::mat4 scale(glm::scale(
            glm::mat4(),
            glm::vec3(scale_)));
   const glm::mat4 rotate(glm::rotate(
            glm::rotate(
               glm::mat4(),
               rotate_,
               glm::vec3(0, 1, 0)
               ),
            -90.0f,
            glm::vec3(1, 0, 0)));
   return glm::mat4(translate * scale * rotate);
} 

Drawable Bush::drawable() const {
   std::vector<glm::mat4> model_matrices;
   model_matrices.push_back(calculateModel());
   return Drawable({draw_template_, model_matrices});
} 


void Bush::shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, bool betterShadow) {
      const glm::mat4 translate(glm::translate(
            glm::mat4(),
            position_));
   const glm::mat4 scale(glm::scale(
            glm::mat4(),
            glm::vec3(scale_)));
   const glm::mat4 rotate(glm::rotate(
            glm::rotate(
               glm::mat4(),
               rotate_,
               glm::vec3(0, 1, 0)
               ),
            -90.0f,
            glm::vec3(1, 0, 0)));
   const glm::mat4 model_matrix(translate * scale * rotate);

   if(betterShadow)
      setupBetterShadowShader(shader, uniform_locations, sunDir, model_matrix);
   else
      setupShadowShader(shader, uniform_locations, sunDir, model_matrix);
   shader.drawMesh(draw_template_.mesh);
}

void Bush::rustle(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::RUSTLE,
         false,
         position_);
   rustle_time_ = 0;
}

bool Bush::isBlocker() {
   return false;
}

void Bush::performObjectHit(SoundEngine& sound_engine) {
   rustle(sound_engine);
}
