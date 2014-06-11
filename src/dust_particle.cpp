#include "particle.h"
#include "dust_particle.h"

#define DUST_MAX 0.01f
#define DUST_MIN -0.01f

namespace {
   glm::vec3 getRandomVec(float aMax, float aMin) {
      float rx = aMin + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (aMax - aMin));
      float ry = aMin + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (aMax - aMin));
      float rz = aMin + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (aMax - aMin));

      return glm::vec3(rx, ry, rz);
   }
}

Dust::Dust(const Mesh& mesh, TextureType texture_type, const glm::vec3& origin, int numParticles) : 
      draw_template_({
            ShaderType::DEFERRED,
            mesh,
            Material(),
            Texture(texture_type, DIFFUSE_TEXTURE),
            boost::none,
            EffectSet() }),
      origin_(origin),
      velocity_(glm::vec3(0.0f, 0.0f, 0.0f)),
      acceleration_(glm::vec3(0.0f, -0.00001f, 0.0f)) {
         
         for (int i = 0; i < numParticles; i++) {
            glm::vec3 rand = getRandomVec(DUST_MAX, DUST_MIN);
            particles_.push_back(Particle(origin_, 0.3f, 0.0f,
               glm::vec3(velocity_.x + rand.x, velocity_.y + rand.y, velocity_.z + rand.z), acceleration_));
         }
      }

void Dust::step(units::MS dt) {
   for (std::vector<Particle>::iterator iter = particles_.begin(); iter != particles_.end();) {
      auto& particle = *iter;
      if (particle.getLife() > 500) {
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
   int numParticles = rand() % 3 + 1;
   for (int i = 0; i < numParticles; i++) {
      glm::vec3 randX = getRandomVec(DUST_MAX, DUST_MIN);
      glm::vec3 randZ = getRandomVec(0.0f, -0.01);
      glm::vec3 randY = getRandomVec(0.01f, 0.001f);
      particles_.push_back(Particle(origin_, 0.3f, 0.0f,
         glm::vec3(velocity_.x + randX.x, velocity_.y + randY.y, velocity_.z + randZ.z), acceleration_));
   }
}
