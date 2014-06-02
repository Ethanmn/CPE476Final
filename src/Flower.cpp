#include "Flower.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

Flower::Flower(const Mesh& mesh, const glm::vec3& position, const GroundPlane& ground, float scale) :
   position_(position.x, ground.heightAt(position) - mesh.min.y, position.z), 
   scale_(scale),
   rotate_(rand() % 360),
   bounding_rectangle_(BoundingRectangle(glm::vec2(position.x, position.z), glm::vec2(2.0f, 2.0f), 90.0f)),
   eaten(false),
   model_(
         glm::translate(
            glm::mat4(),
            position_) *
         glm::scale(
            glm::mat4(),
            glm::vec3(scale_)) *
         glm::rotate(
            glm::mat4(),
            rotate_,
            glm::vec3(0, 1, 0))) {}

void Flower::eat(SoundEngine& sound_engine) {
   sound_engine.playSoundEffect(
         SoundEngine::SoundEffect::EAT_FLOWER,
         false,
         position_);
   eaten = true;
}

void Flower::performObjectHit(SoundEngine&) {
}

glm::mat4 Flower::calculateModel() const {
   return model_;
}
