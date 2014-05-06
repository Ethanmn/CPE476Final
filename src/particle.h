#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/glm.hpp>

#include "graphics/mesh.h"
#include "units.h"
#include "graphics/texture_2d.h"

struct Shader;

struct Particle {
   Partilce(const Mesh& mesh, const glm::vec3& position);

   void step(units::MS dt);
   
   void setAccel(float x, float y ,float z);

   glm::vec3 getPos();
   glm::vec3 getVel();
   glm::vec3 getAccel();
   units::MS getLife();

   void draw(
       Shader& shader,
       const UniformLocationMap& uniform_location_map,
       const glm::mat4& view_matrix) const;

   private:
      Mesh mesh_;
      Texture2D texture_;

      glm::vec3 position_;
      glm::vec3 velocity_;
      glm::vec3 acceleration_;

      float scale_;
      float rotate

      units::MS life_time_;

};

#endif // PARTICLE_H_