#ifndef DEER_H_
#define DEER_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"
#include "Camera.h"
#include "graphics/mesh.h"
#include "graphics/location_maps.h"
#include "units.h"

struct Shader;

struct Deer {
   Deer(const Mesh& mesh, const glm::vec3& position);

   void draw(Shader& shader, const UniformLocationMap& locations) const;
   void step(units::MS dt, const Camera& camera);

   void walkForward();
   void walkBackward();
   void stopWalking();

   void strafeLeft();
   void strafeRight();
   void stopStrafing();

  private:
   enum class WalkDirection {
      FORWARD,
      BACKWARD,
      NONE
   };
   enum class StrafeDirection {
      LEFT,
      RIGHT,
      NONE
   };

   Mesh mesh_;
   glm::vec2 position_;
   glm::vec2 velocity_;
   WalkDirection walk_direction_;
   StrafeDirection strafe_direction_;

   BoundingRectangle bounding_rectangle_;
};

#endif // DEER_H_
