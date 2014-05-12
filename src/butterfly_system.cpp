#include "particle.h"
#include "butterfly_system.h"
#include <stdlib.h>

#define MAX 0.005f
#define MIN -0.005f

#define Y_MAX 0.00007f

ButterflySystem::ButterflySystem(const glm::vec3& origin, int numParticles,
         const AttributeLocationMap& attribute_location_map_, MeshLoader& mesh_loader_) :
            texture_(texture_path(Textures::BUTTERFLY))
            origin_(origin),
            scale_(0.3f),
            velocity_(glm::vec3(0.001f, 0.0f, 0.0f)),
            acceleration_(glm::vec3(0.0f, 0.0f, 0.0f)) {
               auto mesh = (Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh("../models/butterfly.dae")));
               for (int i = 0; i < numParticles; i++) {
                  float rvx = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
                  float rvz = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
                  float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
                  float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
                  float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
                  particles_.push_back(Particle(mesh, glm::vec3(origin_.x + rx, origin_.y + ry, origin_.z + rz), scale_,
                                       glm::vec3(rvx, 0.0f, rvz), acceleration_, texture));
               }
            }

void ButterflySystem::step(units::MS dt) {
   for (auto& particle : particles_) {
      float rvy = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / Y_MAX);

      if (!((particle.getLife() / 100) % 2)) {
         float rvx = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
         float rvz = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
         
         particle.setVel(rvx, rvy * dt, rvz);
      }
      else {
         particle.setVel(particle.getVel().x, -(rvy * dt), particle.getVel().z);
      }
      particle.step(dt);
   }
}

void ButterflySystem::draw(Shader& shader,
         const UniformLocationMap& uniform_location_map,
         const glm::mat4& view_matrix) {
   for (auto& particle : particles_) {
      particle.draw(shader, uniform_location_map, view_matrix);
   }
}
