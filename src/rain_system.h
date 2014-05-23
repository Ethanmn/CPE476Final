#ifndef RAIN_SYSTEM_H_
#define RAIN_SYSTEM_H_

#include "particle.h"
#include "units.h"
#include "graphics/location_maps.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/texture.h"
#include "graphics/draw_template.h"

struct RainSystem {
   RainSystem(const Mesh& mesh, const glm::vec3& origin, int numParticles);

   void step(units::MS dt);

   void reset();
  
   DrawTemplate draw_template() { return draw_template_; }
   Drawable drawable() const;

   private:
      std::vector<Particle> particles_;
      DrawTemplate draw_template_;
      glm::vec3 origin_;
      float scale_;
      glm::vec3 velocity_;
      glm::vec3 acceleration_;
};

#endif // RAIN_SYSTEM_H_
