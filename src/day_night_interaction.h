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
      draw_template_moon_({ShaderType::TEXTURE, mesh, 
            Texture(texture_path(Textures::MOON_STONE)), boost::none, true }),
      draw_template_sun_({ShaderType::TEXTURE, mesh, 
            Texture(texture_path(Textures::SUN_STONE)), boost::none, true }),
      bounding_rectangle_sun_(glm::vec2(-30.0f, -30.0f), glm::vec2(8.0f, 8.0f),
            0.0f),
      bounding_rectangle_moon_(glm::vec2(20.0f, 20.0f), glm::vec2(8.0f, 8.0f),
            0.0f),
      moon_transform_(glm::translate(glm::mat4(),
               glm::vec3(-30.0f, ground.heightAt(glm::vec3(-30, 0, -30)) + 3, -30.0f))),
      sun_transform_(glm::translate(glm::mat4(),
               glm::vec3(20.0f, ground.heightAt(glm::vec3(20, 0, 20)) + 3, 20.0f)))
   { }

   BoundingRectangle bounding_rectangle_sun() const { return bounding_rectangle_sun_; }
   BoundingRectangle bounding_rectangle_moon() const { return bounding_rectangle_moon_; }

   glm::mat4 calculateModel() const;
   Drawable drawableSun() const;
   Drawable drawableMoon() const;


   private:
   DrawTemplate draw_template_moon_;
   DrawTemplate draw_template_sun_;
   BoundingRectangle bounding_rectangle_sun_;
   BoundingRectangle bounding_rectangle_moon_;
   glm::mat4 moon_transform_, sun_transform_;
};

#endif // DAY_NIGHT_INTERACTION_H_
