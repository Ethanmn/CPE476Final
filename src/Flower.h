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
   Flower(const Mesh& mesh, const glm::vec3& position, const GroundPlane& ground, float scale);

   void eat(SoundEngine& sound_engine);

   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }
   BoundingRectangle getBoundingRectangle() {
      return bounding_rectangle_;
   }

   glm::mat4 calculateModel() const;
   bool isEaten() const { return eaten; }  
   bool isBlocker() { return false; }
   void performObjectHit(SoundEngine& sound_engine); 

  private:
   glm::vec3 position_;
   float scale_;
   float rotate_;
   BoundingRectangle bounding_rectangle_;
   bool eaten;
   glm::mat4 model_;
};

#endif
