#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include "shader_setup.h"
#include <iostream>

using namespace std;
namespace {
   bool debug = false;
}

// Setup common to both Texture/Sun Shaders
void DrawShader::setupTexture(Shader& shader, FrameBufferObject shadow_map_fbo_, glm::mat4 viewMatrix, glm::vec3 deerPos,
      glm::vec3 sunDir, float sunIntensity, int lightning) {
   shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 1);
   shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniforms, shadow_map_fbo_.texture_slot());
   sendShadowInverseProjectionView(shader, uniforms, sunDir, deerPos);

   shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
   shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);

   shader.sendUniform(Uniform::LIGHTNING, uniforms, lightning);
   setupSunShader(shader, uniforms, sunIntensity, sunDir); 
}

void DrawShader::setupReflectionShader(Shader& shader, glm::mat4 viewMatrix,
      glm::vec3 sunDir, float sunIntensity, int lightning) {
   shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 0);

   shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
   shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);

   shader.sendUniform(Uniform::LIGHTNING, uniforms, lightning);
   setupSunShader(shader, uniforms, sunIntensity, sunDir); 
}

void DrawShader::Draw(FrameBufferObject shadow_map_fbo_, FrameBufferObject reflection_fbo,
      vector<CulledDrawable> culledDrawables, glm::mat4 viewMatrix, glm::vec3 deerPos,
      glm::vec3 sunDir, float sunIntensity, int lightning) {
   for(auto& shader_pair : shaders.getMap()) {
      Shader& shader = shader_pair.second;
      shader.use();
      switch (shader_pair.first) {
         case ShaderType::SHADOW:
            if(!debug) {
               shadow_map_fbo_.bind();
               glClear(GL_DEPTH_BUFFER_BIT);
            }

            for (auto& drawable : culledDrawables) {
               if (drawable.draw_template.effects.count(EffectType::CASTS_SHADOW)) {
                  for(auto& mt : drawable.model_transforms) {
                     setupShadowShader(shader, uniforms, sunDir, deerPos, mt.model);
                     shader.drawMesh(drawable.draw_template.mesh);
                  }
               }
            }

            if(!debug) {
               glBindFramebuffer(GL_FRAMEBUFFER, 0);
               shadow_map_fbo_.BindForReading();
            }
            break;
         case ShaderType::REFLECTION:
            reflection_fbo.bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            setupReflectionShader(shader, viewMatrix, sunDir, sunIntensity, lightning);
            {
               std::vector<CulledDrawable> reflected;
               const auto scale = glm::scale(glm::mat4(), glm::vec3(1, -1, 1));
               for (auto& drawable : culledDrawables) {
                  if (drawable.draw_template.effects.count(EffectType::CASTS_REFLECTION)) {
                     reflected.push_back(drawable);
                     for (auto& mt : reflected.back().model_transforms) {
                        mt.model = scale * mt.model;
                     }
                  }
               }
               drawTextureShader(shader, reflected, viewMatrix);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            break;
         case ShaderType::TEXTURE:
            {
               std::vector<CulledDrawable> drawables;
               int culledObjects = 0;
               int nonCulledObjects = 0;

               for (auto& drawable : culledDrawables) {
                  CulledDrawable culledDrawable;
                  for (auto& modelTransforms : drawable.model_transforms) {
                     if (!modelTransforms.cullFlag.count(CullType::VIEW_CULLING)) {
                        culledDrawable.model_transforms.push_back(modelTransforms);
                        nonCulledObjects++;
                     }
                     else
                        culledObjects++;
                  }
                  drawables.push_back(culledDrawable);
               }
               std::cout << "culled: " << culledObjects << " non-culled: " << nonCulledObjects << std::endl;
               setupTexture(shader, shadow_map_fbo_, viewMatrix, deerPos, sunDir, sunIntensity, lightning);
               drawTextureShader(shader, drawables, viewMatrix);
            }
            break;
         case ShaderType::WATER:
            shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
            shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);
            shader.sendUniform(Uniform::SCREEN_WIDTH, uniforms, kScreenWidthf);
            shader.sendUniform(Uniform::SCREEN_HEIGHT, uniforms, kScreenHeightf);
            for (auto& drawable : culledDrawables) {
               if (drawable.draw_template.shader_type == ShaderType::WATER) {
                  setupTextureShader(shader, uniforms, *drawable.draw_template.texture);
                  drawModelTransforms(shader, drawable, viewMatrix);
               }
            }
            break;
         case ShaderType::SKYBOX:
            break;
      }
   }
}

void DrawShader::drawModelTransforms(Shader& shader, const CulledDrawable& drawable, const glm::mat4& view) {
   for(const auto& mt : drawable.model_transforms) {
      setupModelView(shader, uniforms, mt.model, view, true);
      shader.drawMesh(drawable.draw_template.mesh);
   }
}

void DrawShader::drawTextureShader(Shader& shader, std::vector<CulledDrawable> drawables, glm::mat4 viewMatrix) {
   for (auto& drawable : drawables) {
      if (drawable.draw_template.shader_type == ShaderType::TEXTURE) {
         { // Per-Drawable Texture Shader Setup
            if (drawable.draw_template.height_map) 
               setupHeightMap(shader, uniforms, *drawable.draw_template.height_map);
            else
               shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniforms, 0);

            if (drawable.draw_template.has_bones()) {
               shader.sendUniform(Uniform::HAS_BONES, uniforms, 1);
               shader.sendUniform(Uniform::BONES, uniforms,
                     drawable.draw_template.mesh.animation.calculateBoneTransformations(
                        drawable.draw_template.mesh.bone_array));
            }
            else
               shader.sendUniform(Uniform::HAS_BONES, uniforms, 0);

            if(drawable.draw_template.texture)
               setupTextureShader(shader, uniforms, *drawable.draw_template.texture);
            else {
               shader.sendUniform(Uniform::HAS_TEXTURE, uniforms, 0);
               shader.sendUniform(Uniform::TEXTURE, uniforms, 0);
            }

            drawable.draw_template.mesh.material.sendMaterial(shader, uniforms);
         }

         drawModelTransforms(shader, drawable, viewMatrix);

         { // Per-drawable Texture Shader teardown
            shader.sendUniform(Uniform::HAS_BONES, uniforms, 0);
         }
      }
   }
}
