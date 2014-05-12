#include "particle.h"
#include "dust_particle.h"

Dust::Dust(const glm::vec3& origin, int numParticles,
	       const AttributeLocationMap& attribute_location_map_, MeshLoader& mesh_loader_,
	       const Texture& texture) :
      origin_(origin),
      scale_(1.0f),
      velocity_(glm::vec3(1.0f, 0.0f, 0.0f)),
      acceleration_(glm::vec3(-0.1f, 1.0f, 0.0f)) {
         auto mesh = (Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh("../models/cube.obj")));
         for (int i = 0; i < numParticles; i++) {
            particles_.push_back(Particle(mesh, origin_, scale_, velocity_, acceleration_, texture));
         }
      }

void Dust::step(units::MS dt) {
   for (auto& particle : particles_) {
      particle.step(dt);
      if (particle.getLife() > 300.0f)
      {
         particle.setMeshMaterial(0.0f, 1.0f, 0.0f);
      }
   }
}

