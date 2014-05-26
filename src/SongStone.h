#ifndef SONG_STONE_H_
#define SONG_STONE_H_

#include <glm/glm.hpp>
#include "bounding_rectangle.h"
#include "GameObject.h"
#include "ground_plane.h"
#include "graphics/mesh.h"
#include "units.h"

struct Shader;
struct SoundEngine;

struct SongStone : GameObject {
   SongStone(const Mesh& mesh, const glm::vec3& position, const GroundPlane& ground, float scale) :
      position_(position.x, ground.heightAt(position) - 2*mesh.min.y, position.z), 
      scale_(scale),
      rotate_(90.0f),
      bounding_rectangle_(BoundingRectangle(glm::vec2(position.x, position.z), glm::vec2(2.0f, 2.0f), 90.0f)) 
   {}

   void playSongStone(SoundEngine& sound_engine);

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
