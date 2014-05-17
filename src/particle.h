#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/glm.hpp>
#include <boost/optional.hpp>
#include "graphics/mesh.h"
#include "units.h"
#include "graphics/texture.h"

struct Shader;

struct Particle {
   Particle(const glm::vec3& position, float scale, const glm::vec3& velocity,
         const glm::vec3& accleration) :
      //mesh_(mesh),
      //texture_(texture),
      position_(position),
      velocity_(velocity),
      acceleration_(accleration),
      scale_(scale),
      rotate_(0.0f),
      life_time_(0) 
{
   //draw_template_.mesh.material = Material(glm::vec3(0.7, 0.24, 0.15));
}

   void step(units::MS dt);

   void setAccel(float x, float y ,float z);

   void setVel(float x, float y ,float z);

   void setPos(float x, float y ,float z);

   glm::vec3 getPos();
   glm::vec3 getVel();
   glm::vec3 getAccel();
   units::MS getLife();

   glm::mat4 calculateModel() const;

  private:
   //Mesh mesh_;
   //boost::optional<Texture> texture_;

   glm::vec3 position_;
   glm::vec3 velocity_;
   glm::vec3 acceleration_;

   float scale_;
   float rotate_;

   units::MS life_time_;
};

#endif // PARTICLE_H_
