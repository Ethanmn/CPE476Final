#ifndef DAY_NIGHT_INTERACTION_H_
#define DAY_NIGHT_INTERACTION_H_

#include <glm/gtc/matrix_transform.hpp>
#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "ground_plane.h"
#include "bounding_rectangle.h"

struct Shader;
struct Shaders;

struct DayNightInteraction {
   DayNightInteraction(const Mesh& mesh, const GroundPlane& ground) :
      mesh_(mesh), 
      moon_texture_(texture_path(Textures::MOON_STONE)),
      sun_texture_(texture_path(Textures::SUN_STONE)),
      bounding_rectangle_start_(glm::vec2(-30.0f, -30.0f), glm::vec2(8.0f, 8.0f),
            0.0f),
      bounding_rectangle_stop_(glm::vec2(20.0f, 20.0f), glm::vec2(8.0f, 8.0f),
            0.0f),
      start_transform_(glm::translate(glm::mat4(),
               glm::vec3(-30.0f, ground.heightAt(glm::vec3(-30, 0, -30)) + 3, -30.0f))),
      stop_transform_(glm::translate(glm::mat4(),
               glm::vec3(20.0f, ground.heightAt(glm::vec3(20, 0, 20)) + 3, 20.0f)))
   { }

   void drawStop(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix);
   void drawStart(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix);
   BoundingRectangle bounding_rectangle_stop() const { return bounding_rectangle_stop_; }
   BoundingRectangle bounding_rectangle_start() const { return bounding_rectangle_start_; }

  private:
   Mesh mesh_;
   Texture moon_texture_;
   Texture sun_texture_;
   BoundingRectangle bounding_rectangle_start_;
   BoundingRectangle bounding_rectangle_stop_;
   glm::mat4 start_transform_, stop_transform_;
};

#endif // DAY_NIGHT_INTERACTION_H_
