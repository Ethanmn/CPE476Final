#include "particle.h"
#include "rain_system.h"
#include "graphics/shader_setup.h"
#include <stdlib.h>

RainSystem::RainSystem(const glm::vec3& origin, int numParticles,
         const AttributeLocationMap& attribute_location_map_, MeshLoader& mesh_loader_) :
            texture_(texture_path(Textures::BUTTERFLY)),
            origin_(origin),
            scale_(1.0f),
            velocity_(glm::vec3(0.0f, -0.0001f, 0.0f)),
            acceleration_(glm::vec3(0.0f, -0.0001f, 0.0f)) {
               auto mesh = (Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh("../models/cube.obj")));
               for (int i = 0; i < numParticles; i++) {
                  float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 100.0f);
                  float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50.0f);
                  float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 100.0f);
                  particles_.push_back(Particle(mesh, glm::vec3(origin_.x + rx, origin_.y + ry, origin_.z + rz), scale_,
                                       velocity_, acceleration_, texture_));
               }
            }

void RainSystem::step(units::MS dt) {
   for (auto& particle : particles_) {
      if (particle.getLife() >= 5000) {
         float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
         float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
         float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
         particle.setVel(0.0f, -0.0001f, 0.0f);
         particle.setPos(origin_.x + rx, origin_.y + ry, origin_.z + rz);
      }
      particle.step(dt);
   }
}

void RainSystem::draw(Shader& shader,
         const UniformLocationMap& uniform_location_map,
         const glm::mat4& view_matrix,
         float sunIntensity) {
   setupTextureShader(shader, uniform_location_map, sunIntensity, texture_.textureID());
   texture_.enable();

   for (auto& particle : particles_) {
      particle.draw(shader, uniform_location_map, view_matrix);
   }

   texture_.disable();
}
