#include "SongStone.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

void SongStone::playSongStone(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::WOODPECKER0,
         false,
         position_);
}

void SongStone::performObjectHit(SoundEngine& sound_engine) {
   playSongStone(sound_engine);
}

glm::mat4 SongStone::calculateModel() const {
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
               glm::vec3(1, 0, 0)
               ),
            -90.0f,
            glm::vec3(1, 0, 0)));
   return glm::mat4(translate * scale * rotate);
} 


