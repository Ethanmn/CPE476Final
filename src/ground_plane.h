#ifndef GROUND_PLANE_H_
#define GROUND_PLANE_H_

#include "graphics/draw_template.h"
#include "glm/glm.hpp"
#include "graphics/location_maps.h"
#include "graphics/texture.h"

#include <Magick++.h>
#include <vector>

#include <Magick++.h>
#include <vector>

struct Shader;
struct Shaders;

struct GroundPlane {
   static const int GROUND_SCALE;
   GroundPlane(const Mesh& mesh);

   float heightAt(const glm::vec3& position) const;

   DrawTemplate draw_template() const { return draw_template_; }
   Drawable drawable() const;

  private:
   DrawTemplate draw_template_;
   std::vector<glm::mat4> transforms_;
   Magick::Image height_map_image_;
};

#endif // GROUND_PLANE_H_
