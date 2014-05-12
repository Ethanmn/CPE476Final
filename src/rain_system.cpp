#include "particle.h"
#include "rain_system.h"
#include "graphics/shader_setup.h"
#include <stdlib.h>

#define RAIN_MAX 200.0f
#define RAIN_MIN -200.0f

RainSystem::RainSystem(const glm::vec3& origin, int numParticles,
         const AttributeLocationMap& attribute_location_map_, MeshLoader& mesh_loader_) :
            texture_(texture_path(Textures::BUTTERFLY)),
            origin_(origin),
            scale_(0.3f),
            velocity_(glm::vec3(0.0f, 0.0f, 0.0f)),
            acceleration_(glm::vec3(0.0f, -0.00001f, 0.0f)) {
               auto mesh = (Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh("../models/cube.obj")));
               for (int i = 0; i < numParticles; i++) {
                  float rx = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
                  float ry = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
                  float rz = RAIN_MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (RAIN_MAX - RAIN_MIN));
                  particles_.push_back(Particle(mesh, glm::vec3(origin_.x + rx, origin_.y + ry + 150, origin_.z + rz), scale_,
                                       velocity_, acceleration_, texture_));
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

void RainSystem::draw(Shader& shader,
         const UniformLocationMap& uniform_location_map,
         const glm::mat4& view_matrix) {

   for (auto& particle : particles_) {
      Material(glm::vec3(0.1, 0.4f, 0.9f)).sendMaterial(shader, uniform_location_map);
      particle.draw(shader, uniform_location_map, view_matrix);
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
