#ifndef DEER_H_
#define DEER_H_

#include <glm/glm.hpp>

#include "Camera.h"
#include "graphics/mesh.h"
#include "units.h"

struct Shader;
struct UniformLocationMap;

struct Deer {
   Deer(const Mesh& mesh, const glm::vec3 position) :
      mesh_(mesh),
      position_(position),
      velocity_(0.0f),
      walk_direction_(WalkType::NONE),
      strafe_direction_(StrafeType::NONE) {}

   void draw(Shader& shader, const UniformLocationMap& model_locations) const;
   void step(units::MS dt, const Camera& camera);

   void walkForward();
   void walkBackward();
   void stopWalking();

   void strafeLeft();
   void strafeRight();
   void stopStrafing();

  private:
   enum class WalkType {
      FORWARD,
      BACKWARD,
      NONE
   };
   enum class StrafeType {
      LEFT,
      RIGHT,
      NONE
   };

   Mesh mesh_;
   glm::vec3 position_;
   glm::vec3 velocity_;
   WalkType walk_direction_;
   StrafeType strafe_direction_;
};

#endif // DEER_H_
