#ifndef DAY_NIGHT_INTERACTION_H_
#define DAY_NIGHT_INTERACTION_H_

#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "bounding_rectangle.h"

struct Shader;
struct Shaders;

struct DayNightInteraction {
   DayNightInteraction(const Mesh& mesh) : 
            mesh_(mesh), 
            bounding_rectangle_start_(glm::vec2(-30.0f, -30.0f), glm::vec2(8.0f, 8.0f),
            0.0f),
            bounding_rectangle_stop_(glm::vec2(20.0f, 20.0f), glm::vec2(8.0f, 8.0f),
            0.0f)
   {
      mesh_.material = Material(glm::vec3(0.45, 0.24, 0.15));
   }

   void drawStop(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix);
   void drawStart(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix);
   BoundingRectangle bounding_rectangle_stop() const { return bounding_rectangle_stop_; }
   BoundingRectangle bounding_rectangle_start() const { return bounding_rectangle_start_; }

   private:
      Mesh mesh_;
      BoundingRectangle bounding_rectangle_start_;
      BoundingRectangle bounding_rectangle_stop_;
};

#endif // DAY_NIGHT_INTERACTION_H_
