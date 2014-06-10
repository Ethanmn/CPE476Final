#include "particle.h"
#include "butterfly_system.h"
#include <stdlib.h>

#define MAX 0.005f
#define MIN -0.005f

#define Y_MAX 0.00007f

const float SCALE_MAX = 0.3f * 100.0f;
const float SCALE_MIN = 0.05f * 100.0f;

const float DISAPPEAR_INTERVAL = 1000.0f;

ButterflySystem::ButterflySystem(const Mesh& mesh, TextureType texture_type, const glm::vec3& origin, int numParticles) :
   draw_template_({
      ShaderType::DEFERRED,
      mesh, 
      Material(),
      Texture(texture_type, DIFFUSE_TEXTURE),
      boost::none,
      EffectSet({EffectType::CASTS_SHADOW, EffectType::CASTS_REFLECTION, EffectType::IS_FIREFLY, EffectType::IS_GOD_RAY})
   }),
   origin_(origin),
   velocity_(glm::vec3(0.001f, 0.0f, 0.0f)),
   acceleration_(glm::vec3(0.0f, 0.0f, 0.0f)),
   num_particles_(numParticles),
   timer(0.0f)
{}

void ButterflySystem::generate(const glm::vec3& origin, int numParticles) {
   origin_ = origin;
   num_particles_ = numParticles;
   timer = 0.0f;
   for (int i = 0; i < numParticles; i++) {
      float rvx = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
      float rvz = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
      float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
      float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
      float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
      particles_.push_back(Particle(glm::vec3(origin_.x + rx, origin_.y + ry, origin_.z + rz), 
         (rand() % (int)((SCALE_MAX - SCALE_MIN) + SCALE_MIN))  / 100.0f, rand() % 360,
         glm::vec3(rvx, 0.0f, rvz), acceleration_));
   }
}

void ButterflySystem::step(units::MS dt) {
   if (isEmpty()) {
      return;
   }

   for (auto& particle : particles_) {
      float rvy = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / Y_MAX);

      if (!((particle.getLife() / 100) % 2)) {
         float rvx = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
         float rvz = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
         
         particle.setVel(rvx, rvy, rvz);
      }
      else {
         particle.setVel(particle.getVel().x, -(rvy), particle.getVel().z);
      }
      particle.step(dt);
   }

   if (timer > DISAPPEAR_INTERVAL) {
      particles_.pop_back();
      num_particles_--;
      timer = 0.0f;
   }
   else {
      timer += dt;
   }
}

bool ButterflySystem::checkIfEmpty(const ButterflySystem& system) {
   return system.isEmpty();
}

Drawable ButterflySystem::drawable() const {
   std::vector<DrawInstance> model_matrices;
   for (auto& particle : particles_) 
      model_matrices.push_back(particle.calculateModel());
   return Drawable({draw_template_, model_matrices});
}

bool ButterflySystem::isEmpty() const {
   return num_particles_ == 0;
}
