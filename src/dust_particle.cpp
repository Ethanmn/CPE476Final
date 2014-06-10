#include "particle.h"
#include "dust_particle.h"

#define DUST_MAX 0.01f
#define DUST_MIN -0.01f

namespace {
   glm::vec3 getRandomVec() {
      float rx = DUST_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (DUST_MAX - DUST_MIN));
      float ry = DUST_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (DUST_MAX - DUST_MIN));
      float rz = DUST_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (DUST_MAX - DUST_MIN));

      return glm::vec3(rx, ry, rz);
   }
}

Dust::Dust(const Mesh& mesh, TextureType texture_type, const glm::vec3& origin, int numParticles) : 
      draw_template_({
            ShaderType::DEFERRED,
            mesh,
            Material(),
            boost::none,
            boost::none,
            EffectSet() }),
      origin_(origin),
      velocity_(glm::vec3(0.0f, 0.0f, 0.0f)),
      acceleration_(glm::vec3(0.0f, -0.00001f, 0.0f)) {
         glm::vec3 rand = getRandomVec();
         for (int i = 0; i < numParticles; i++) {
            particles_.push_back(Particle(origin_, 0.3f, 0.0f,
               glm::vec3(velocity_.x + rand.x, velocity_.y + rand.y, velocity_.z + rand.z), acceleration_));
         }
      }

void Dust::step(units::MS dt) {
   for (std::vector<Particle>::iterator iter = particles_.begin(); iter != particles_.end();) {
      auto& particle = *iter;
      if (particle.getLife() > 700) {
         iter = particles_.erase(iter);
      }
      else {
         particle.step(dt);
         iter++;
      }
      
   }
}

Drawable Dust::drawable() const {
   std::vector<DrawInstance> insts;
   for (auto& p : particles_) {
      insts.push_back(p.calculateModel());
   }
   return Drawable({
         draw_template_,
         insts
         });
}

void Dust::add(glm::vec3 origin) {
   origin_ = origin;
   int numParticles = rand() % 10 + 5;
   for (int i = 0; i < numParticles; i++) {
      glm::vec3 rand = getRandomVec();
      particles_.push_back(Particle(origin_, 0.3f, 0.0f,
         glm::vec3(velocity_.x + rand.x, velocity_.y + rand.y + 0.001f, velocity_.z + rand.z), acceleration_));
   }
}
