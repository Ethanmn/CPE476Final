#ifndef BUSH_H_
#define BUSH_H_

#include <glm/glm.hpp>
#include "bounding_rectangle.h"
#include "GameObject.h"
#include "ground_plane.h"
#include "graphics/mesh.h"
#include "units.h"

struct Shader;
struct SoundEngine;

struct Bush : public GameObject {
   Bush(const Mesh& mesh, const glm::vec3& position, const GroundPlane& ground, float scale, units::MS rustle_time) :
      position_(position.x, ground.heightAt(position) - mesh.min.y, position.z),
      scale_(scale),
      rotate_(0.0f),
      elapsed_time_(0),
      rustle_time_(rustle_time),
      kMaxRustleTime(rustle_time),
      bounding_rectangle_(BoundingRectangle(
               glm::vec2(position.x, position.z),
               glm::vec2(8.0f, 8.0f),
               0.0f)) {}


   void step(units::MS dt);
   void rustle(SoundEngine& sound_engine);
   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   BoundingRectangle getBoundingRectangle() {
      return bounding_rectangle_;
   }

   bool isBlocker();
   void performObjectHit(SoundEngine& sound_engine);

   glm::mat4 calculateModel() const;


  private: 
   glm::vec3 position_;
   float scale_;
   float rotate_;
   units::MS elapsed_time_, rustle_time_;
   const units::MS kMaxRustleTime;
   BoundingRectangle bounding_rectangle_;
};

#endif // BUSH_H_
