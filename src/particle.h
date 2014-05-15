#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/glm.hpp>

#include "graphics/mesh.h"
#include "units.h"
#include "graphics/texture.h"

struct Shader;

struct Particle {
   Particle(const Mesh& mesh, const glm::vec3& position, float scale, const glm::vec3& velocity,
         const glm::vec3& accleration, const Texture& texture) :
      mesh_(mesh),
      texture_(texture),
      position_(position),
      velocity_(velocity),
      acceleration_(accleration),
      scale_(scale),
      rotate_(0.0f),
      life_time_(0) {
   mesh_.material = Material(glm::vec3(0.7, 0.24, 0.15));
}

   void step(units::MS dt);

   void draw(
         Shader& shader,
         const UniformLocationMap& uniform_location_map,
         const glm::mat4& view_matrix) const;

   void setAccel(float x, float y ,float z);

   void setVel(float x, float y ,float z);

   void setPos(float x, float y ,float z);

   void setMeshMaterial(float x, float y ,float z);

   glm::vec3 getPos();
   glm::vec3 getVel();
   glm::vec3 getAccel();
   units::MS getLife();



  private:
   Mesh mesh_;
   Texture texture_;

   glm::vec3 position_;
   glm::vec3 velocity_;
   glm::vec3 acceleration_;

   float scale_;
   float rotate_;

   units::MS life_time_;
};

#endif // PARTICLE_H_
