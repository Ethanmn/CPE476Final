/*
   Tree.h
   Katie Keim
   CPE 476 - Deer
*/
#ifndef TREE_H_
#define TREE_H_

#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "ground_plane.h"
#include "bounding_rectangle.h"
#include "GameObject.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "sound_engine.h"


struct Tree : public GameObject {
   Tree(glm::vec3 position);

   BoundingRectangle getBoundingRectangle() {
      return bRect;
   }

   bool isBlocker();
   void performObjectHit(SoundEngine& sound_engine);

   glm::mat4 calculateModel() const;

   void step(units::MS dt);

   private:
      BoundingRectangle bRect;
      float rotate_;
      units::MS elapsed_time_, rustle_time_;
      const units::MS kMaxRustleTime;
      glm::mat4 translate_scale_;
};

#endif //TREE_H_
