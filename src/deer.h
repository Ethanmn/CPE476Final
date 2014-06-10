#ifndef DEER_H_
#define DEER_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"
#include "graphics/draw_template.h"
#include "graphics/location_maps.h"
#include "units.h"

struct GroundPlane;
struct Shader;
struct Flower;
struct SoundEngine;

struct Deer {
  private:
   struct ModelState {
      glm::vec3 position;
      glm::vec3 velocity;
      glm::vec2 last_facing;
      float current_lean;
      float current_rock;
   };
  public:
   Deer(const Mesh& walk_mesh, const Mesh& eat_mesh, const Mesh& sleep_mesh, const Mesh& pounce_mesh, const glm::vec3& position);

   BoundingRectangle getNextBoundingBox(units::MS dt);

   void step(units::MS dt, const GroundPlane& ground, SoundEngine& sound_engine);
   void walkForward();
   void walkBackward();
   void stopWalking();

   void turnLeft();
   void turnRight();
   void stopTurning();

   void jump();
   void eat(Flower& flower);
   void pounce(const glm::vec2& pounce_target);
   void stop_pounce() { pounce_target_ = boost::none; spring_ = false; }

   glm::vec3 getPosition() const;
   glm::vec3 getFacing() const;

   bool is_sleeping() const { return sleeping_; }
   bool is_eating() const { return eating_; }
   bool is_pouncing() const { return pounce_target_; }

   bool isMoving();

   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }
   BoundingRectangle head_bounding_rectangle() const {
      BoundingRectangle head(bounding_rectangle_);
      head.set_dimensions(
            glm::vec2(
               bounding_rectangle_.getDimensions().x / 3,
               bounding_rectangle_.getDimensions().y));
      head.set_position(
            bounding_rectangle_.getCenter() +
            bounding_rectangle_.getDimensions().x/2.f * glm::vec2(
               glm::sin(glm::radians(bounding_rectangle_.getRotation() + 90.f)),
               glm::cos(glm::radians(bounding_rectangle_.getRotation() + 90.f)))
            );
      return head;
   }

   BoundingRectangle front_feet_bounding_rectangle() const {
      BoundingRectangle head(bounding_rectangle_);
      head.set_dimensions(
            glm::vec2(
               bounding_rectangle_.getDimensions().x / 6,
               bounding_rectangle_.getDimensions().y));
      head.set_position(
            bounding_rectangle_.getCenter() +
            bounding_rectangle_.getDimensions().x/5.f * glm::vec2(
               glm::sin(glm::radians(bounding_rectangle_.getRotation() + 90.f)),
               glm::cos(glm::radians(bounding_rectangle_.getRotation() + 90.f)))
            );
      return head;
   }
   BoundingRectangle back_feet_bounding_rectangle() const {
      BoundingRectangle head(bounding_rectangle_);
      head.set_dimensions(
            glm::vec2(
               bounding_rectangle_.getDimensions().x / 8,
               bounding_rectangle_.getDimensions().y));
      head.set_position(
            bounding_rectangle_.getCenter() +
            bounding_rectangle_.getDimensions().x/5.f * glm::vec2(
               glm::sin(glm::radians(bounding_rectangle_.getRotation() + 90.f) - 3.0),
               glm::cos(glm::radians(bounding_rectangle_.getRotation() + 90.f) - 3.0))
            );
      return head;
   }

   void block();

   void changeRock(units::MS dt, const GroundPlane& ground_plane);
   glm::mat4 calculateModel(const ModelState& model_state) const;
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

   BoundingRectangle boundingRectangleFromModel(const ModelState& model_state) const;

   glm::vec3 acceleration() const;
   glm::vec3 predictVelocity(units::MS dt, const glm::vec3& acceleration) const;
   glm::vec2 predictFacing(units::MS dt) const;
   glm::vec3 predictPosition(units::MS dt, const glm::vec3& velocity) const;

   DrawTemplate draw_template_;
   bool eating_;
   bool sleeping_;
   boost::optional<glm::vec2> pounce_target_;
   bool spring_; // for the pounce yah
   Mesh walk_mesh_;
   Mesh eat_mesh_, sleep_mesh_, pounce_mesh_;

   ModelState model_state_;

   float desired_lean_;
   float desired_rock_;

   WalkDirection walk_direction_;
   TurnDirection turn_direction_;
   BoundingRectangle bounding_rectangle_;

   units::MS step_timer_;

   bool is_jumping_;
   bool is_walking_;
   bool blocked;

   glm::mat4 pivot_, inverse_pivot_;
   boost::optional<Flower&> flower_;
};

#endif // DEER_H_
