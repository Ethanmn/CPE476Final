#ifndef GOD_RAYS_H_
#define GOD_RAYS_H_

#include <glm/gtc/matrix_transform.hpp>
#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "ground_plane.h"
#include "bounding_rectangle.h"
#include "graphics/draw_template.h"
struct Shader;
struct Shaders;

struct GodRays {
   GodRays(const Mesh& mesh, const glm::vec3& position, float scale) :
      position_(position),
      next_position_(position),
      current_scale_(glm::vec3(1.0)), 
      scale_(scale),
      rotate_(45.0),
      draw_template_({ShaderType::FINAL_LIGHT_PASS, mesh,
            boost::none,
            boost::none,
            EffectSet({EffectType::IS_GOD_RAY}), 
            }),
      bounding_rectangle_(glm::vec2(position_.x, position_.z), glm::vec2(5.0f, 5.0f), 0.0f)
      { 
         draw_template_.mesh.material = Material(glm::vec3(1.0, 1.0, 1.0)); 
      }

   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   void setRayPositions(glm::vec2 current_ray, glm::vec2 next_ray);
   void setCurrentRayScale(float scale_amount);

   glm::mat4 calculateModel(glm::vec3 position, glm::vec3 scaleAmount) const;
   Drawable drawable() const;


   private:
   glm::vec3 position_;
   glm::vec3 next_position_;
   glm::vec3 current_scale_;
   float scale_;
   float rotate_;
   DrawTemplate draw_template_;
   BoundingRectangle bounding_rectangle_;
};

#endif 
