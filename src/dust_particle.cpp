#include "particle.h"
#include "dust_particle.h"

Dust::Dust(const Mesh& mesh, const glm::vec3& origin, int numParticles) : 
      draw_template_({ShaderType::DEFERRED, mesh, boost::none, boost::none, EffectSet() }),
      origin_(origin),
      scale_(1.0f),
      velocity_(glm::vec3(1.0f, 0.0f, 0.0f)),
      acceleration_(glm::vec3(-0.1f, 1.0f, 0.0f)) {
         for (int i = 0; i < numParticles; i++) {
            particles_.push_back(Particle(origin_, scale_, 0.0f, velocity_, acceleration_));
         }
      }

void Dust::step(units::MS dt) {
   for (auto& particle : particles_) {
      particle.step(dt);
      if (particle.getLife() > 300.0f)
      {
        // particle.setMeshMaterial(0.0f, 1.0f, 0.0f);
      }
   }
}

