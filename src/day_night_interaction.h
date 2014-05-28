#ifndef DAY_NIGHT_INTERACTION_H_
#define DAY_NIGHT_INTERACTION_H_

#include <glm/gtc/matrix_transform.hpp>
#include "graphics/mesh.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "ground_plane.h"
#include "bounding_rectangle.h"
#include "graphics/draw_template.h"
struct Shader;
struct Shaders;

struct DayNightInteraction {
   DayNightInteraction(const Mesh& mesh, const GroundPlane& ground) :
      moon_drawable_({
            DrawTemplate({
               ShaderType::TEXTURE,
               mesh,
               Texture(TextureType::MOON_STONE, DIFFUSE_TEXTURE),
               boost::none,
               EffectSet({EffectType::CASTS_SHADOW})
               }),
            std::vector<glm::mat4>({
               glm::translate(
                  glm::mat4(),
                  glm::vec3(-30.0f, ground.heightAt(glm::vec3(-30, 0, -30)) + 3, -30.0f))
               }),
            }),
      sun_drawable_({
            DrawTemplate({
               ShaderType::TEXTURE, mesh,
               Texture(TextureType::SUN_STONE, DIFFUSE_TEXTURE),
               boost::none,
               EffectSet({EffectType::CASTS_SHADOW})
               }),
            std::vector<glm::mat4>({
               glm::translate(
                  glm::mat4(),
                  glm::vec3(20.0f, ground.heightAt(glm::vec3(20, 0, 20)) + 3, 20.0f))
               }),
            }),
      bounding_rectangle_sun_(glm::vec2(20.0f, 20.0f), glm::vec2(8.0f, 8.0f),
            0.0f),
      bounding_rectangle_moon_(glm::vec2(-30.0f, -30.0f), glm::vec2(8.0f, 8.0f),
            0.0f)
   { }

   BoundingRectangle bounding_rectangle_sun() const { return bounding_rectangle_sun_; }
   BoundingRectangle bounding_rectangle_moon() const { return bounding_rectangle_moon_; }

   Drawable drawableSun() const { return moon_drawable_; }
   Drawable drawableMoon() const { return sun_drawable_; }

  private:
   Drawable moon_drawable_;
   Drawable sun_drawable_;
   BoundingRectangle bounding_rectangle_sun_;
   BoundingRectangle bounding_rectangle_moon_;
};

#endif // DAY_NIGHT_INTERACTION_H_
