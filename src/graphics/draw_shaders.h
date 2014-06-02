#ifndef DRAW_SHADERS_
#define DRAW_SHADERS_

#include <glm/gtc/matrix_transform.hpp>   
#include "draw_template.h"
#include "graphics/day_cycle.h"
#include <vector>
#include "graphics/gl_adapters/frame_buffer_object.h"
#include "globals.h"

struct DrawShader {
   DrawShader() : uniforms(shaders.getUniformLocationMap()), 
      projectionMatrix(glm::perspective(kFieldOfView, kScreenWidthf/kScreenHeightf, kNear, kFar)) {}

   void Draw(const FrameBufferObject& shadow_map_fbo_, 
            const FrameBufferObject& reflection_fbo, 
            const std::vector<CulledDrawable>& culledDrawables,
            const glm::mat4& viewMatrix, 
            int useBlinnPhong, 
            const glm::vec3& deerPos,
            const glm::vec3& sunDir, 
            float sunIntensity, 
            int lightning);

   void SunInformation(const glm::vec3& direction, float intensity);
   Shaders getShaders() const { return shaders; }

  private:
   void setupTexture(Shader& shader,
            const FrameBufferObject& shadow_map_fbo_,
            const glm::mat4& viewMatrix, 
            int useBlinnPhong, 
            const glm::vec3& deerPos, 
            const glm::vec3& sunDir, 
            float sunIntensity, 
            int lightning);

   void drawTextureShader(Shader& shader, 
            const std::vector<Drawable>& drawables, 
            const glm::mat4& viewMatrix);

   void sendOutputShaderType(ShaderType shaderT, Shader& shader);

   void SendHeightMap(Shader& shader, const Drawable& drawable);
   void SendBones(Shader& shader, const Drawable& drawable);
   void SendTexture(Shader& shader, const Drawable& drawable);

   void setupReflectionShader(Shader& shader, 
            const glm::mat4& viewMatrix,
            const glm::vec3& sunDir, 
            float sunIntensity, 
            int lightning);

   void drawModelTransforms(Shader& shader, 
            const Drawable& drawable,
            const glm::mat4& view,
            bool needsModel);

   Shaders shaders;
   UniformLocationMap uniforms;
   const glm::mat4 projectionMatrix;
   TextureCache texture_cache_;
};


#endif
