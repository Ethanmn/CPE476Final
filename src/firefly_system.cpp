#include "particle.h"
#include "firefly_system.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>

#define VMAX 0.005f
#define VMIN -0.005f
#define MAX 0.075f
#define MIN -0.075f

#define Y_MAX 0.00007f

FireflySystem::FireflySystem(const Mesh& mesh, const Mesh& glow_mesh, 
      TextureType texture_type, const glm::vec3& origin, int numParticles) :
            draw_template_({
                  ShaderType::DEFERRED,
                  mesh, 
                  Material(),
                  Texture(texture_type, DIFFUSE_TEXTURE),
                  boost::none,
                  EffectSet({EffectType::CASTS_SHADOW, EffectType::CASTS_REFLECTION})
                  }),
            draw_template_glow_({
                  ShaderType::FINAL_LIGHT_PASS,
                  glow_mesh, 
                  Material(),
                  Texture(texture_type, DIFFUSE_TEXTURE),
                  boost::none,
                  EffectSet({EffectType::IS_FIREFLY, EffectType::IS_GOD_RAY})
                  }),
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
                  particles_.push_back(Particle(glm::vec3(origin_.x + rx, origin_.y + ry, origin_.z + rz), scale_, rand(),
                                       glm::vec3(rvx, 0.0f, rvz), acceleration_));

               }
               //draw_template_.mesh.material = Material(glm::vec3(0.7, 0.24, 0.15));
            }


void FireflySystem::step(units::MS dt) {
   for (auto& particle : particles_) {
      float rvy = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / Y_MAX);

      if (!((particle.getLife() / 100) % 2)) {
         float rvx = VMIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (VMAX - VMIN));
         float rvz = VMIN + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (VMAX - VMIN));
         
         particle.setVel(rvx, rvy, rvz);
      }
      else {
         particle.setVel(particle.getVel().x, -(rvy), particle.getVel().z);
      }
      particle.step(dt);
   }
}

Drawable FireflySystem::drawable() const {
   std::vector<DrawInstance> model_matrices;
   for (auto& particle : particles_) 
      model_matrices.push_back(particle.calculateModel());
   return Drawable({draw_template_, model_matrices});
}

Drawable FireflySystem::drawable_glow() const {
   std::vector<DrawInstance> model_matrices;
   for (auto& particle : particles_) 
      model_matrices.push_back(particle.calculateModel() * glm::scale(glm::mat4(), glm::vec3(3.0f)));
   return Drawable({draw_template_glow_, model_matrices});
}
