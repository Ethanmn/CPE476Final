#include "particle.h"
#include "leaf_system.h"
#include <stdlib.h>

#define LEAF_MAX 20.0f
#define LEAF_MIN -20.0f
#define ROT_MAX 360.0f

namespace {
   glm::vec3 getRandomVec() {
      float rx = LEAF_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (LEAF_MAX - LEAF_MIN));
      float ry = LEAF_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (LEAF_MAX - LEAF_MIN));
      float rz = LEAF_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (LEAF_MAX - LEAF_MIN));

      return glm::vec3(rx, ry, rz);
   }
}

LeafSystem::LeafSystem(const Mesh& mesh, TextureType texture_type, const glm::vec3& origin, int numParticles) : 
            draw_template_({
                  ShaderType::TEXTURE,
                  mesh, 
                  Material(),
                  Texture(texture_type, DIFFUSE_TEXTURE),
                  boost::none,
                  EffectSet({EffectType::CASTS_SHADOW, EffectType::CASTS_REFLECTION})
                  }),
            origin_(origin),
            scale_(0.7f),
            velocity_(glm::vec3(0.0f, 0.0f, 0.0f)),
            acceleration_(glm::vec3(0.0f, -0.000001f, 0.0f)) {
               for (int i = 0; i < numParticles; i++) {
                  glm::vec3 randVec = getRandomVec();
                  float randAngle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ROT_MAX));
                  particles_.push_back(Particle(glm::vec3(origin_.x + randVec.x, origin_.y + randVec.y, origin_.z + randVec.z), 
                              scale_, randAngle, velocity_,  acceleration_));
               }
            }

void LeafSystem::step(units::MS dt) {
   for (auto& particle : particles_) {
      if (particle.getPos().y <= 1.0f) {
         particle.setPos(particle.getPos().x, 1.0f, particle.getPos().z);
         particle.setVel(0.0f, 0.0f, 0.0f);
         particle.setAccel(0.0f, 0.0f, 0.0f);
      }
      else {
         particle.setRot(particle.getRot() + 10.0f * (dt / 100));
      }
      particle.step(dt);
   }
}

void LeafSystem::reset() {
   for (auto& particle : particles_) {
      glm::vec3 randVec = getRandomVec();
      particle.setVel(0.0f, 0.0f, 0.0f);
      particle.setPos(origin_.x + randVec.x, origin_.y + randVec.y, origin_.z + randVec.z);
   }
}

void LeafSystem::add() {
   int numAdd = rand() % 10 + 4;
   for (int i = 0; i < numAdd; i++) {
      glm::vec3 randVec = getRandomVec();
      float randAngle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ROT_MAX));
      particles_.push_back(Particle(glm::vec3(origin_.x + randVec.x, origin_.y + randVec.y, origin_.z + randVec.z), 
      scale_, randAngle, velocity_,  acceleration_));
   }
}

Drawable LeafSystem::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for (auto& particle : particles_) 
      model_matrices.push_back(particle.calculateModel());
   return Drawable({draw_template_, model_matrices});
}
