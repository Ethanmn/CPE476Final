#ifndef FIREFLY_SYSTEM_H_
#define FIREFLY_SYSTEM_H_

#include "particle.h"
#include "units.h"
#include "graphics/location_maps.h"
#include "graphics/assimp/mesh_loader.h"
#include "graphics/texture.h"
#include "graphics/draw_template.h"

struct FireflySystem {

   FireflySystem(const Mesh& mesh, const Mesh& glow_mesh,
      TextureType texture_type, const glm::vec3& origin, int numParticles);
   void step(units::MS dt);

   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;
   Drawable drawable_glow() const;

   private:
      std::vector<Particle> particles_;
      DrawTemplate draw_template_;
      DrawTemplate draw_template_glow_;
      glm::vec3 origin_;
      float scale_;
      glm::vec3 velocity_;
      glm::vec3 acceleration_;
};

#endif // FIREFLY_SYSTEM_H_
