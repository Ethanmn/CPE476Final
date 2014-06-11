#include "Flower.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

Flower::Flower(const Mesh& mesh, const glm::vec3& position, const GroundPlane& ground, float scale, float errX, float errY) :
   position_(position.x, ground.heightAt(position) - mesh.min.y - scale / 2, position.z),
   bounding_rectangle_(BoundingRectangle(glm::vec2(position.x, position.z), glm::vec2(scale, scale), 0.0f)),
   eaten(false),
   model_(
      glm::translate(
         glm::mat4(),
         position_
      ) *
      glm::scale(
         glm::mat4(),
         glm::vec3(scale)
      ) *
      glm::rotate(
         glm::mat4(),
         float(rand() % 360),
         glm::vec3(0, 1.0f, 0)
      ) *
      glm::translate(
         glm::mat4(),
         glm::vec3(errX, 0.0f, errY)
      )
   ) 
{}

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
