#ifndef LIGHTNING_INTERACTION_H_
#define LIGHTNING_INTERACTION_H_

#include <glm/gtc/matrix_transform.hpp>
#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "ground_plane.h"
#include "bounding_rectangle.h"
#include "graphics/draw_template.h"
struct Shader;
struct Shaders;

struct Lightning {
   Lightning(const Mesh& mesh, const glm::vec3& position, float scale) :
      position_(position.x, position.y, position.z), 
      scale_(scale),
      rotate_(0),
      draw_template_({ShaderType::TEXTURE, mesh,
            Texture(TextureType::LIGHTNING, DIFFUSE_TEXTURE),
            boost::none,
            EffectSet({EffectType::CASTS_SHADOW})
            }),
      bounding_rectangle_(glm::vec2(position_.x, position_.z), glm::vec2(5.0f, 5.0f), 0.0f)
      { }

   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   glm::mat4 calculateModel() const;
   Drawable drawable() const;


   private:
   glm::vec3 position_;
   float scale_;
   float rotate_;
   DrawTemplate draw_template_;
   BoundingRectangle bounding_rectangle_;
};

#endif // DAY_NIGHT_INTERACTION_H_
