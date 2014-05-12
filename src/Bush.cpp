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

void Bush::draw(
      Shader& shader,
      const UniformLocationMap& uniform_location_map,
      const glm::mat4& view_matrix) const {
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

   setupModelView(shader, uniform_location_map, model_matrix, view_matrix, true);

   mesh_.material.sendMaterial(shader, uniform_location_map);

   shader.drawMesh(mesh_);
   bounding_rectangle_.draw(uniform_location_map, shader, 0, view_matrix);
}

void Bush::shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, glm::vec3 deerPos, bool betterShadow) {
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
      setupBetterShadowShader(shader, uniform_locations, sunDir, deerPos, model_matrix);
   else
      setupShadowShader(shader, uniform_locations, sunDir, deerPos, model_matrix);
   shader.drawMesh(mesh_);
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