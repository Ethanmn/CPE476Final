#ifndef DEER_H_
#define DEER_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"
#include "Camera.h"
#include "graphics/draw_template.h"
#include "graphics/location_maps.h"
#include "units.h"

struct GroundPlane;
struct Shader;
struct SoundEngine;


struct Deer {
   Deer(const Mesh& mesh, const glm::vec3& position);

   void draw(Shader& shader, const UniformLocationMap& locations,
             const glm::mat4& viewMatrix) const;
   BoundingRectangle getNextBoundingBox(units::MS dt, const Camera& camera);

   void step(units::MS dt, const Camera& camera, const GroundPlane& ground, SoundEngine& sound_engine);
   void walkForward();
   void walkBackward();
   void stopWalking();

   void strafeLeft();
   void strafeRight();
   void stopStrafing();

   void jump();

   glm::vec3 getPosition() const;
   glm::vec3 getFacing() const;

   bool isMoving();
   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   void block();
   /* helper functions for shadows */
   void shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir);

   glm::mat4 calculateModel() const;
   DrawTemplate draw_template() const { return draw_template_; }

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

   bool has_acceleration() const {
      return walk_direction_ != WalkDirection::NONE || strafe_direction_ != StrafeDirection::NONE;
   }

   glm::vec3 acceleration(const Camera& camera) const;
   glm::vec3 predictVelocity(units::MS dt, const glm::vec3& acceleration) const;
   glm::vec2 predictFacing(const glm::vec3& velocity) const;
   glm::vec3 predictPosition(units::MS dt, const glm::vec3& velocity) const;

   DrawTemplate draw_template_;
   glm::vec3 position_;
   glm::vec3 velocity_;
   glm::vec2 last_facing_;
   float desired_lean_;
   float current_lean_;

   WalkDirection walk_direction_;
   StrafeDirection strafe_direction_;
   BoundingRectangle bounding_rectangle_;

   units::MS step_timer_;

   bool is_jumping_;
   bool is_walking_;
   bool is_strafing_;
   bool blocked;
};

#endif // DEER_H_
