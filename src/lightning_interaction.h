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

struct LightningInteraction {
   LightningInteraction(const Mesh& mesh, const GroundPlane& ground) :
      draw_template_({ShaderType::TEXTURE, mesh,
            Texture(TextureType::SUN_STONE, DIFFUSE_TEXTURE),
            boost::none,
            EffectSet({EffectType::CASTS_SHADOW}) }),
      bounding_rectangle_(glm::vec2(-30.0f, -30.0f), glm::vec2(8.0f, 8.0f),
            0.0f)
   { }

   BoundingRectangle bounding_rectangle() const { return bounding_rectangle_; }

   glm::mat4 calculateModel() const;
   Drawable drawable() const;


   private:
   DrawTemplate draw_template_;
   BoundingRectangle bounding_rectangle_;
};

#endif // DAY_NIGHT_INTERACTION_H_
