#ifndef DRAW_SHADERS_
#define DRAW_SHADERS_

#include <glm/gtc/matrix_transform.hpp>   
#include "draw_template.h"
#include "graphics/day_cycle.h"
#include <vector>
#include "graphics/gl_adapters/frame_buffer_object.h"
#include "graphics/gl_adapters/deferred_frame_buffer.h"
#include "globals.h"

struct DrawShader {
   DrawShader() : uniforms(shaders.getUniformLocationMap()) {}

   void Draw(const FrameBufferObject& shadow_map_fbo_, 
            const FrameBufferObject& reflection_fbo,

            const FrameBufferObject& deferred_diffuse_fbo_,
            const FrameBufferObject& deferred_position_fbo_,
            const FrameBufferObject& deferred_normal_fbo_,

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
   void drawTextureShader(bool isReflection, Shader& shader, 
            const std::vector<Drawable>& drawables, 
            const glm::mat4& viewMatrix, 
            const glm::vec3& sunDir, 
            float sunIntensity, 
            int lightning,
            const FrameBufferObject& reflection_fbo);

   void SendDeferred(Shader& shader, 
            const UniformLocationMap& uniforms,
            const DeferredFrameBuffer& deferred_fbo_);

   void sendOutputShaderType(ShaderType shaderT, Shader& shader);

   void SendShadow(Shader& shader, const UniformLocationMap& uniforms,
            const FrameBufferObject& shadow_map_fbo_,
            const glm::vec3& deerPos, const glm::vec3& sunDir);
   void SendHeightMap(Shader& shader, const Drawable& drawable);
   void SendBones(Shader& shader, const Drawable& drawable);
   void SendTexture(Shader& shader, const Drawable& drawable);
   void drawModelTransforms(Shader& shader, 
            const Drawable& drawable,
            const glm::mat4& view,
            bool needsModel,
            const UniformLocationMap& uniforms);
   void drawModelTransforms(Shader& shader, 
            const Drawable& drawable,
            const glm::mat4& view,
            const FrameBufferObject& deferred_diffuse_fbo_,
            const FrameBufferObject& deferred_position_fbo_,
            const FrameBufferObject& deferred_normal_fbo_,
            bool needsModel,
            const UniformLocationMap& uniforms);

   Shaders shaders;
   UniformLocationMap uniforms;
   TextureCache texture_cache_;
};


#endif
