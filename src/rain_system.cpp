#include "particle.h"
#include "rain_system.h"
#include <stdlib.h>

#define RAIN_MAX_XZ 200.0f
#define RAIN_MIN_XZ -200.0f
#define RAIN_MAX_Y 300.0f
#define RAIN_MIN_Y 50.0f

namespace {
   glm::vec3 getRandomVec() {
      float rx = RAIN_MIN_XZ + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX_XZ - RAIN_MIN_XZ));
      float ry = RAIN_MIN_Y + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX_Y - RAIN_MIN_Y));
      float rz = RAIN_MIN_XZ + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX_XZ - RAIN_MIN_XZ));

      return glm::vec3(rx, ry, rz);
   }
}


RainSystem::RainSystem(const Mesh& mesh, const glm::vec3& origin, int numParticles) : 
            draw_template_({ShaderType::DEFERRED, mesh, 
                  Texture(TextureType::RAIN, DIFFUSE_TEXTURE), boost::none, EffectSet() }),
            origin_(origin),
            scale_(0.3f),
            velocity_(glm::vec3(0.0f, 0.0f, 0.0f)),
            acceleration_(glm::vec3(0.0f, -0.00001f, 0.0f)) {
               for (int i = 0; i < numParticles; i++) {
                  glm::vec3 randVec = getRandomVec();
                  particles_.push_back(Particle(glm::vec3(origin_.x + randVec.x, origin_.y + randVec.y, origin_.z + randVec.z), 
                              scale_, 0.0f, velocity_, acceleration_));
               }
            }

void RainSystem::step(units::MS dt) {
   for (auto& particle : particles_) {
      if (particle.getPos().y <= -5.0f) {
         glm::vec3 randVec = getRandomVec();
         particle.setVel(0.0f, 0.0f, 0.0f);
         particle.setPos(origin_.x + randVec.x, origin_.y + randVec.y, origin_.z + randVec.z);
      }
      particle.step(dt);
   }
}

void RainSystem::reset() {
   for (auto& particle : particles_) {
      glm::vec3 randVec = getRandomVec();
      particle.setVel(0.0f, 0.0f, 0.0f);
      particle.setPos(origin_.x + randVec.x, origin_.y + randVec.y, origin_.z + randVec.z);
   }
}

Drawable RainSystem::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for (auto& particle : particles_) 
      model_matrices.push_back(particle.calculateModel());
   return Drawable({draw_template_, model_matrices});
}
