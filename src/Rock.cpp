/* 
   Rock.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "Rock.h"
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/material.h"
#include "sound_engine.h"

Rock::Rock(const Mesh& mesh, const glm::vec3& position, float angleOffset, const GroundPlane& ground, 
      float scale, int density) :
   density_(density),
   bounding_rectangle_(BoundingRectangle(
            glm::vec2(position.x, position.z),
            glm::vec2(2.0f * scale),
            0.0f)),
   scale(scale),
   translate_scale_(
         glm::translate(glm::mat4(), glm::vec3(position.x, ground.heightAt(position) - mesh.min.y, position.z)) *
         glm::scale(glm::mat4(), glm::vec3(scale))),
   default_model_(
      translate_scale_ *
      glm::rotate(
         glm::rotate(
            glm::mat4(),
            angleOffset,
            glm::vec3(0, 1.0f, 0)
            ),
         -90.0f,
         glm::vec3(1.0f, 0, 0)
      )
   )
{}

glm::mat4 Rock::calculateModel() const {
      return default_model_;
} 

bool Rock::isBlocker() {
   return scale > 0.5;
}

void Rock::performObjectHit(SoundEngine& sound_engine) {
}
