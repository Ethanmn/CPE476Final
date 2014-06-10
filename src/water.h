#ifndef WATER_H_
#define WATER_H_

#include "globals.h"
#include "graphics/gl_adapters/frame_buffer_object.h"
#include "graphics/draw_template.h"

#include "ground_plane.h"

#include <glm/gtc/matrix_transform.hpp>

struct Water {
   Water(const Mesh& mesh) :
      mesh_(mesh),
      reflection_fbo_(kScreenWidth, kScreenHeight, REFLECTION_TEXTURE, FBOType::COLOR_WITH_DEPTH)
   {}

   Drawable drawable() const {
      return Drawable({
            DrawTemplate({
               ShaderType::FINAL_LIGHT_PASS,
               mesh_,
               Material(),
               boost::none,
               boost::none,
               EffectSet({EffectType::IS_WATER})
               }),
            std::vector<DrawInstance>({glm::scale(glm::mat4(), glm::vec3(
                     GroundPlane::GROUND_SCALE, 1, GroundPlane::GROUND_SCALE))})
            });
   }

   FrameBufferObject fbo() const { return reflection_fbo_; }

  private:
   Mesh mesh_;
   FrameBufferObject reflection_fbo_;
};

#endif // WATER_H_
