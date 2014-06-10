#ifndef DUST_PARTICLE_H_
#define DUST_PARTICLE_H_

#include "particle.h"
#include "units.h"
#include "graphics/location_maps.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/texture.h"
#include "graphics/draw_template.h"

struct Dust {
   Dust(const Mesh& mesh, TextureType texture_type, const glm::vec3& origin, int numParticles);

   void step(units::MS dt);

   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;
   void add(glm::vec3 origin);
   
   private:
      DrawTemplate draw_template_;
      std::vector<Particle> particles_;
      glm::vec3 origin_;
      glm::vec3 velocity_;
      glm::vec3 acceleration_;
};

#endif // DUST_PARTICLE_H_
