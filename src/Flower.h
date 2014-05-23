#ifndef FLOWER_H_
#define FLOWER_H_

/*
 * http://stackoverflow.com/questions/2874441/deleting-elements-from-stl-set-while-iterating
 */
#include <glm/glm.hpp>
#include "bounding_rectangle.h"
#include "GameObject.h"
#include "ground_plane.h"
#include "graphics/mesh.h"
#include "units.h"

struct Shader;
struct SoundEngine;

struct Flower : GameObject {
   Flower(const glm::vec3& position, float scale) :
      position_(position), 
      scale_(scale),
      rotate_(0.0f),
      bounding_rectangle_(BoundingRectangle(glm::vec2(position.x, position.z), glm::vec2(2.0f, 2.0f), 0.0f)) 
   {}

   void eatFlower(SoundEngine& sound_engine);

   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }
   BoundingRectangle getBoundingRectangle() {
      return bounding_rectangle_;
   }

   glm::mat4 calculateModel() const;
   
   bool isBlocker() { return false; }
   void performObjectHit(SoundEngine& sound_engine); 

   private:
      glm::vec3 position_;
      float scale_;
      float rotate_;
      BoundingRectangle bounding_rectangle_;

};

#endif
