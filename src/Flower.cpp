#include "Flower.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

void Flower::eatFlower(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::EAT_FLOWER,
         false,
         position_);
}

void Flower::performObjectHit(SoundEngine& sound_engine) {
   eatFlower(sound_engine);
}

glm::mat4 Flower::calculateModel() const {
   const glm::mat4 translate(glm::translate(
            glm::mat4(),
            position_));
   const glm::mat4 scale(glm::scale(
            glm::mat4(),
            glm::vec3(scale_)));
   const glm::mat4 rotate(glm::rotate(
            glm::mat4(),
            rotate_,
            glm::vec3(0, 1, 0)));
   return glm::mat4(translate * scale * rotate);
} 


