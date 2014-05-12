#ifndef DEER_H_
#define DEER_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"
#include "Camera.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/location_maps.h"
#include "units.h"
#include "graphics/texture.h"

struct GroundPlane;
struct Shader;

struct Deer {
   Deer(const Mesh& mesh, const glm::vec3& position);

   void draw(Shader& shader, const UniformLocationMap& locations,
             const glm::mat4& viewMatrix) const;

   void step(units::MS dt, const Camera& camera, const GroundPlane& ground_plane);

   void walkForward();
   void walkBackward();
   void stopWalking();

   void strafeLeft();
   void strafeRight();
   void stopStrafing();

   void jump();

   glm::vec3 getPosition();

   bool isMoving();
   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   /* helper functions for shadows */
   void shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, bool betterShadow);

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
   Texture texture_;
   glm::vec3 position_;
   glm::vec3 velocity_;
   glm::vec3 last_facing_;

   WalkDirection walk_direction_;
   StrafeDirection strafe_direction_;
   BoundingRectangle bounding_rectangle_;

   bool is_jumping_;
   bool is_walking_;
   bool is_strafing_;
};

#endif // DEER_H_
