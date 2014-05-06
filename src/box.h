#ifndef BOX_H_
#define BOX_H_

#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "bounding_rectangle.h"

struct Shader;
struct Shaders;

struct Box {
   Box(const Mesh& mesh) : 
            mesh_(mesh), 
            bounding_rectangle_green_(glm::vec2(-30.0f, -30.0f), glm::vec2(8.0f, 8.0f),
            0.0f),
            bounding_rectangle_red_(glm::vec2(20.0f, 20.0f), glm::vec2(8.0f, 8.0f),
            0.0f)
   {
      mesh_.material = Material(glm::vec3(0.45, 0.24, 0.15));
   }

   void drawRed(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix);
   void drawGreen(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix);
   BoundingRectangle bounding_rectangle_red() const { return bounding_rectangle_red_; }
   BoundingRectangle bounding_rectangle_green() const { return bounding_rectangle_green_; }

   private:
      Mesh mesh_;
      BoundingRectangle bounding_rectangle_green_;
      BoundingRectangle bounding_rectangle_red_;
};

#endif // BOX_H_
