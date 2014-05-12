#ifndef DUST_PARTICLE_H_
#define DUST_PARTICLE_H_

#include "particle.h"
#include "units.h"
#include "graphics/location_maps.h"
#include "graphics/assimp/mesh_loader.h"

struct Dust {
   Dust(const glm::vec3& origin, int numParticles,
	       const AttributeLocationMap& attribute_location_map_, MeshLoader& mesh_loader_,
	       const Texture& texture);

   void step(units::MS dt);
   
   private:
      std::vector<Particle> particles_;
      glm::vec3 origin_;
      float scale_;
      glm::vec3 velocity_;
      glm::vec3 acceleration_;
};

#endif // DUST_PARTICLE_H_