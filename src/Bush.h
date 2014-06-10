#ifndef BUSH_H_
#define BUSH_H_

#include <glm/glm.hpp>
#include "bounding_rectangle.h"
#include "GameObject.h"
#include "ground_plane.h"
#include "graphics/mesh.h"
#include "units.h"
#include "butterfly_system.h"

struct Shader;
struct SoundEngine;

struct Bush : public GameObject {
   Bush(const Mesh& mesh, const glm::vec3& position, float angleOffset, const GroundPlane& ground, 
        float scale, units::MS rustle_time, const Mesh& butterfly);

   void step(units::MS dt);
   void rustle(SoundEngine& sound_engine);
   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   BoundingRectangle getBoundingRectangle() {
      return bounding_rectangle_;
   }

   bool isBlocker();
   void performObjectHit(SoundEngine& sound_engine);

   glm::mat4 calculateModel() const;

  public:
     ButterflySystem butterfly_system_;

  private: 
   float rotate_;
   units::MS elapsed_time_, rustle_time_;
   const units::MS kMaxRustleTime;
   BoundingRectangle bounding_rectangle_;
   glm::mat4 translate_scale_;
   glm::mat4 default_model_;
   bool hasButterflies_;
};

#endif // BUSH_H_
