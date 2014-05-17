#include "particle.h"
#include "butterfly_system.h"
#include "graphics/shader_setup.h"
#include <stdlib.h>

#define MAX 0.005f
#define MIN -0.005f

#define Y_MAX 0.00007f

ButterflySystem::ButterflySystem(const Mesh& mesh, const glm::vec3& origin, int numParticles) :
            draw_template_({ShaderType::TEXTURE, mesh, Texture(texture_path(Textures::DEER))}),
            origin_(origin),
            scale_(0.3f),
            velocity_(glm::vec3(0.001f, 0.0f, 0.0f)),
            acceleration_(glm::vec3(0.0f, 0.0f, 0.0f)) {
               for (int i = 0; i < numParticles; i++) {
                  float rvx = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
                  float rvz = MIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (MAX - MIN));
                  float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
                  float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
                  float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0f);
                  particles_.push_back(Particle(glm::vec3(origin_.x + rx, origin_.y + ry, origin_.z + rz), scale_,
                                       glm::vec3(rvx, 0.0f, rvz), acceleration_));

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
/*
void ButterflySystem::draw(Shader& shader,
         const UniformLocationMap& uniform_location_map,
         const glm::mat4& view_matrix) {
   setupTextureShader(shader, uniform_location_map, texture_);
   texture_.enable();

   for (auto& particle : particles_) {
      particle.draw(shader, uniform_location_map, view_matrix);
   }

   texture_.disable();
}
*/

Drawable ButterflySystem::drawable() const {
   std::vector<glm::mat4> model_matrices;
   for (auto& particle : particles_) 
      model_matrices.push_back(particle.calculateModel());
   return Drawable({draw_template_, model_matrices});
}
