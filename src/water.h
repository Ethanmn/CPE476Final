#ifndef WATER_H_
#define WATER_H_

#include "globals.h"
#include "graphics/gl_adapters/frame_buffer_object.h"
#include "graphics/draw_template.h"

#include <glm/gtc/matrix_transform.hpp>

struct Water {
   Water(const Mesh& mesh) :
      mesh_(mesh),
      reflection_fbo_(kScreenWidth, kScreenHeight, DIFFUSE_TEXTURE, FBOType::COLOR_WITH_DEPTH)
   {}

   Drawable drawable() const {
      return Drawable({
            DrawTemplate({
               ShaderType::WATER,
               mesh_,
               reflection_fbo_.texture(),
               boost::none,
               EffectSet()
               }),
            std::vector<glm::mat4>({glm::scale(glm::mat4(), glm::vec3(10, 1, 10))})
            });
   }

   FrameBufferObject fbo() const { return reflection_fbo_; }

  private:
   Mesh mesh_;
   FrameBufferObject reflection_fbo_;
};

#endif // WATER_H_
