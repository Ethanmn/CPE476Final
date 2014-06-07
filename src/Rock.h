/* 
   Rock.h
   Katie Keim
   Deer - CPE 476
*/
#ifndef ROCK_H_
#define ROCK_H_

#include <glm/glm.hpp>
#include "bounding_rectangle.h"
#include "GameObject.h"
#include "ground_plane.h"
#include "graphics/mesh.h"
#include "units.h"

struct Shader;
struct SoundEngine;

struct Rock : public GameObject {
   Rock(const Mesh& mesh, const glm::vec3& position, float angleOffset, const GroundPlane& ground, 
        float scale);

   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   BoundingRectangle getBoundingRectangle() {
      return bounding_rectangle_;
   }

   bool isBlocker();
   void performObjectHit(SoundEngine& sound_engine);

   glm::mat4 calculateModel() const;


  private: 
     BoundingRectangle bounding_rectangle_;
     float scale;
     glm::mat4 translate_scale_;
     glm::mat4 default_model_;
};

#endif // ROCK_H_
