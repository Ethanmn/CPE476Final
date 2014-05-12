#ifndef TREE_H_
#define TREE_H_

#include <glm/glm.hpp>

#include "bounding_rectangle.h"
#include "ground_plane.h"
#include "graphics/mesh.h"
#include "units.h"

struct Shader;

struct Tree {
   Tree(const Mesh& mesh, const glm::vec3& position, const GroundPlane& ground, float scale, units::MS rustle_time) :
      position_(position.x, ground.heightAt(position) - mesh.min.y, position.z),
      scale_(scale),
      rotate_(0.0f),
      elapsed_time_(0),
      rustle_time_(rustle_time),
      kMaxRustleTime(rustle_time),
      bounding_rectangle_(
            glm::vec2(position.x, position.z),
            glm::vec2(8.0f, 8.0f),
            0.0f),
      mesh_(mesh) {
         mesh_.material = Material(glm::vec3(0.45, 0.24, 0.15));
      }

   void step(units::MS dt);
   void rustle();
   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   void draw(
         Shader& shader,
         const UniformLocationMap& uniform_location_map,
         const glm::mat4& view_matrix) const;
   void shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, glm::vec3 deerPos, bool betterShadow);

  private:
   glm::vec3 position_;
   float scale_;
   float rotate_;
   units::MS elapsed_time_, rustle_time_;
   const units::MS kMaxRustleTime;
   BoundingRectangle bounding_rectangle_;
   Mesh mesh_;
};

#endif // TREE_H_
