#ifndef DEER_H_
#define DEER_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"
#include "graphics/draw_template.h"
#include "graphics/location_maps.h"
#include "units.h"

struct GroundPlane;
struct Shader;
struct SoundEngine;


struct Deer {
   Deer(const Mesh& walk_mesh, const Mesh& eat_mesh, const glm::vec3& position);

   BoundingRectangle getNextBoundingBox(units::MS dt);

   void step(units::MS dt, const GroundPlane& ground, SoundEngine& sound_engine);
   void walkForward();
   void walkBackward();
   void stopWalking();

   void turnLeft();
   void turnRight();
   void stopTurning();

   void jump();
   void eat();

   glm::vec3 getPosition() const;
   glm::vec3 getFacing() const;

   bool isMoving();
   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   void block();

   glm::mat4 calculateModel() const;
   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;

  private:
   enum class WalkDirection {
      FORWARD,
      BACKWARD,
      NONE
   };
   enum class TurnDirection {
      LEFT,
      RIGHT,
      NONE
   };

   bool has_acceleration() const {
      return walk_direction_ != WalkDirection::NONE;
   }

   glm::vec3 acceleration() const;
   glm::vec3 predictVelocity(units::MS dt, const glm::vec3& acceleration) const;
   glm::vec2 predictFacing(units::MS dt) const;
   glm::vec3 predictPosition(units::MS dt, const glm::vec3& velocity) const;

   DrawTemplate draw_template_;
   bool eating_;
   Mesh walk_mesh_;
   Mesh eat_mesh_;

   glm::vec3 up_;

   glm::vec3 position_;
   glm::vec3 velocity_;
   glm::vec2 last_facing_;
   float desired_lean_;
   float current_lean_;

   WalkDirection walk_direction_;
   TurnDirection turn_direction_;
   BoundingRectangle bounding_rectangle_;

   units::MS step_timer_;

   bool is_jumping_;
   bool is_walking_;
   bool blocked;

   glm::mat4 pivot_, inverse_pivot_;
};

#endif // DEER_H_
