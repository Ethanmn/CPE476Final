#include "particle.h"
#include "rain_system.h"
#include <stdlib.h>

#define RAIN_MAX 200.0f
#define RAIN_MIN -200.0f

RainSystem::RainSystem(const Mesh& mesh, const glm::vec3& origin, int numParticles) : 
            draw_template_({ShaderType::TEXTURE, mesh, boost::none, boost::none, EffectSet() }),
            origin_(origin),
            scale_(0.3f),
            velocity_(glm::vec3(0.0f, 0.0f, 0.0f)),
            acceleration_(glm::vec3(0.0f, -0.00001f, 0.0f)) {
               for (int i = 0; i < numParticles; i++) {
                  float rx = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
                  float ry = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
                  float rz = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
                  particles_.push_back(Particle(glm::vec3(origin_.x + rx, origin_.y + ry + 150, origin_.z + rz), 
                              scale_, velocity_, acceleration_));
               }
            }

void RainSystem::step(units::MS dt) {
   for (auto& particle : particles_) {
      if (particle.getPos().y <= -25.0f) {
         float rx = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
         float ry = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
         float rz = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
         particle.setVel(0.0f, 0.0f, 0.0f);
         particle.setPos(origin_.x + rx, origin_.y + ry, origin_.z + rz);
      }
      particle.step(dt);
   }
}

void RainSystem::reset() {
   for (auto& particle : particles_) {
      float rx = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
      float ry = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
      float rz = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
      particle.setVel(0.0f, 0.0f, 0.0f);
      particle.setPos(origin_.x + rx, origin_.y + ry, origin_.z + rz);
   }
}

Drawable RainSystem::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for (auto& particle : particles_) 
      model_matrices.push_back(particle.calculateModel());
   return Drawable({draw_template_, model_matrices});
}
