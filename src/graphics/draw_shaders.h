#ifndef DRAW_SHADERS_
#define DRAW_SHADERS_

#include <glm/gtc/matrix_transform.hpp>   
#include "draw_template.h"
#include "graphics/day_cycle.h"
#include <vector>
#include "graphics/gl_adapters/frame_buffer_object.h"

struct DrawShader {
   DrawShader() : uniforms(shaders.getUniformLocationMap()), 
      projectionMatrix(glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 500.f)) {}

   void Draw(FrameBufferObject shadow_map_fbo_, FrameBufferObject reflection_fbo,
         std::vector<Drawable> drawables, glm::mat4 viewMatrix, glm::vec3 deerPos,
         glm::vec3 sunDir, float sunIntensity, int lightning);
   void SunInformation(glm::vec3 direction, float intensity);
   Shaders getShaders() const { return shaders; }

  private:
   void setupTexture(Shader& shader, FrameBufferObject shadow_map_fbo_, glm::mat4 viewMatrix, glm::vec3 deerPos,
      glm::vec3 sunDir, float sunIntensity, int lightning);
   void drawTextureShader(Shader& shader, std::vector<Drawable> drawables, glm::mat4 viewMatrix);

   void setupReflectionShader(Shader& shader, glm::mat4 viewMatrix,
      glm::vec3 sunDir, float sunIntensity, int lightning);
   void drawModelTransforms(Shader& shader, const Drawable& drawable, const glm::mat4& view);

   Shaders shaders;
   UniformLocationMap uniforms;
   const glm::mat4 projectionMatrix;
};


#endif
