#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include <iostream>
#include <SDL.h>

#include "frustum.h"

using namespace std;
namespace {
   bool debug = false;
   bool printCurrentShaderName = false;
   const float kOrthoProjAmount = 70.0f;
   const glm::mat4 kShadowProjection = glm::ortho(
         -kOrthoProjAmount, kOrthoProjAmount,
         -kOrthoProjAmount, kOrthoProjAmount,
         -40.0f, 40.0f);
   const glm::mat4 biasMatrix(
         0.5, 0.0, 0.0, 0.0,
         0.0, 0.5, 0.0, 0.0,
         0.0, 0.0, 0.5, 0.0,
         0.5, 0.5, 0.5, 1.0);

   void SendSun(Shader& shader, const UniformLocationMap& uniforms,
         float sunIntensity, const glm::vec3& sunDir) {
      glPolygonMode(GL_FRONT, GL_FILL);
      shader.sendUniform(Uniform::SUN_INTENSITY, uniforms, sunIntensity);
      shader.sendUniform(Uniform::SUN_DIR, uniforms, sunDir);
   }

   void SendInverseShadow(Shader& shader, const UniformLocationMap& uniforms,
         const glm::vec3& lightDir, const glm::vec3& deerPos) {
      glm::mat4 lightMat, shadowProjection, shadowView;

      shadowProjection = biasMatrix * kShadowProjection;
      shadowView = glm::lookAt(lightDir + deerPos, deerPos, glm::vec3(0.0, 1.0, 0.0));
      lightMat = shadowProjection * shadowView;

      shader.sendUniform(Uniform::SHADOW_MAP, uniforms, lightMat);
   }
   void SendShadow(Shader& shader, const UniformLocationMap& uniforms,
         const FrameBufferObject& shadow_map_fbo_, 
         const glm::vec3& deerPos, const glm::vec3& sunDir) {
      shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 1);
      shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniforms, shadow_map_fbo_.texture_slot());
      SendInverseShadow(shader, uniforms, sunDir, deerPos);
   }
}

//void DrawShader::SendDeferred(Shader& shader, const UniformLocationMap& uniforms,
         //const DeferredFrameBuffer& deferred_fbo_) {
      //shader.sendUniform(Uniform::FINAL_PASS_DIFFUSE_TEXTURE, uniforms, deferred_fbo_.diffuse_texture_slot());
      ////shader.sendUniform(Uniform::FINAL_PASS_POSITION_TEXTURE, uniforms, deferred_fbo_.position_texture_slot());
      //shader.sendUniform(Uniform::FINAL_PASS_NORMAL_TEXTURE, uniforms, deferred_fbo_.normal_texture_slot());
//} 

void DrawShader::drawModelTransforms(Shader& shader, const Drawable& drawable, 
      const glm::mat4& view, bool needsModel) {
   for(const auto& mt : drawable.model_transforms) {
      glPolygonMode(GL_FRONT, GL_FILL);
      shader.sendUniform(Uniform::MODEL_VIEW, uniforms, view * mt);
      shader.sendUniform(Uniform::NORMAL, uniforms,
               glm::transpose(glm::inverse(view * mt)));
      if(needsModel)
         shader.sendUniform(Uniform::MODEL, uniforms, mt);   
      
      shader.drawMesh(drawable.draw_template.mesh);
   }
}

void DrawShader::SendHeightMap(Shader& shader, const Drawable& drawable) {
   if (drawable.draw_template.height_map) {
      shader.sendUniform(Uniform::HEIGHT_MAP, uniforms, 
         (*drawable.draw_template.height_map).texture_slot());
      shader.sendUniform(Uniform::HEIGHT_MAP_SCALE, uniforms, kHeightMapScale);
      shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniforms, 1);
      (*drawable.draw_template.height_map).enable(texture_cache_);
   }
   else {
      shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniforms, 0);
   }
}

void DrawShader::SendBones(Shader& shader, const Drawable& drawable) {
   if (drawable.draw_template.has_bones()) {
      shader.sendUniform(Uniform::HAS_BONES, uniforms, 1);
      shader.sendUniform(Uniform::BONES, uniforms,
      drawable.draw_template.mesh.animation.calculateBoneTransformations(
      drawable.draw_template.mesh.bone_array));
   }
   else {
      shader.sendUniform(Uniform::HAS_BONES, uniforms, 0);
   }
}

void DrawShader::SendTexture(Shader& shader, const Drawable& drawable) {
   if(drawable.draw_template.texture) {
      shader.sendUniform(Uniform::HAS_TEXTURE, uniforms, 1);
      shader.sendUniform(Uniform::TEXTURE, uniforms, (*drawable.draw_template.texture).texture_slot());
      (*drawable.draw_template.texture).enable(texture_cache_);

   }
   else {
      shader.sendUniform(Uniform::HAS_TEXTURE, uniforms, 0);
      shader.sendUniform(Uniform::TEXTURE, uniforms, 0);
   }
}

void DrawShader::drawTextureShader(Shader& shader, const std::vector<Drawable>& drawables, 
      const glm::mat4& viewMatrix, const glm::vec3& sunDir, float sunIntensity, 
      int lightning,
      const FrameBufferObject& fbo) {

   shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
   shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);

   shader.sendUniform(Uniform::LIGHTNING, uniforms, lightning);
   SendSun(shader, uniforms, sunIntensity, sunDir);


   for (auto& drawable : drawables) {
      if (drawable.draw_template.shader_type == ShaderType::TEXTURE) { 
         { 
         // Per-Drawable Texture Shader Setup
            SendHeightMap(shader, drawable);
            SendBones(shader, drawable);
            SendTexture(shader, drawable);

            if (drawable.draw_template.effects.count(EffectType::USE_DEF_TEXT)) { 
               fbo.texture().enable(texture_cache_);
               shader.sendUniform(Uniform::TEXTURE, uniforms, fbo.texture_slot());
            }

            drawable.draw_template.mesh.material.sendMaterial(shader, uniforms);
         }
         drawModelTransforms(shader, drawable, viewMatrix, true);
      }
   }
}

void DrawShader::Draw(const FrameBufferObject& shadow_map_fbo_, 
                      const FrameBufferObject& reflection_fbo, 
                      const FrameBufferObject& deferred_diffuse_fbo_,
                      const FrameBufferObject& deferred_position_fbo_,
                      const FrameBufferObject& deferred_normal_fbo_,
                      const vector<CulledDrawable>& culledDrawables,
                      const glm::mat4& viewMatrix, 
                      int useBlinnPhong, 
                      const glm::vec3& deerPos,
                      const glm::vec3& sunDir, 
                      float sunIntensity, 
                      int lightning) {

   for(auto& shader_pair : shaders.getMap()) {
      Shader& shader = shader_pair.second;
      shader.use();
      int setOutputType = -1;
      switch (shader_pair.first) {
         case ShaderType::SHADOW:
            if(printCurrentShaderName)
               printf("Shadow\n");
            {
               const auto shadow_view = glm::lookAt(sunDir + deerPos, deerPos, glm::vec3(0.0, 1.0, 0.0));
               const auto view_projection = kShadowProjection * shadow_view;
               // Shadow Culling.
               Frustum frustum(view_projection);
               const auto shadow_drawables = frustum.cullShadowDrawables(culledDrawables);
               if(!debug) {
                  shadow_map_fbo_.bind();
                  glClear(GL_DEPTH_BUFFER_BIT);
               }
               {
                  for (auto& drawable : shadow_drawables) {
                     SendBones(shader, Drawable::fromCulledDrawable(drawable, CullType::SHADOW_CULLING));
                     if (drawable.draw_template.effects.count(EffectType::CASTS_SHADOW)) {
                        for(auto& mt : drawable.model_transforms) {
                           if (!mt.cullFlag.count(CullType::SHADOW_CULLING)) {
                              shader.sendUniform(Uniform::MODEL_VIEW_PROJECTION, uniforms, 
                                    view_projection * mt.model);
                              shader.drawMesh(drawable.draw_template.mesh);
                           }
                        }
                     }
                  }
               }

               if(!debug) {
                  glBindFramebuffer(GL_FRAMEBUFFER, 0);
                  shadow_map_fbo_.texture().enable(texture_cache_);
               }
            }
            break;

         case ShaderType::REFLECTION:
            if(printCurrentShaderName)
               printf("Reflection\n");
            reflection_fbo.bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.sendUniform(Uniform::USE_BLINN_PHONG, uniforms, useBlinnPhong);
            shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 0);
            {
               std::vector<Drawable> reflected;
               const auto scale = glm::scale(glm::mat4(), glm::vec3(1, -1, 1));
               for (auto& drawable : culledDrawables) {
                  Drawable newDrawable;
                  if (drawable.draw_template.effects.count(EffectType::CASTS_REFLECTION)) {
                     reflected.push_back(newDrawable);
                     for (auto& mt : reflected.back().model_transforms) {
                        mt = scale * mt;
                     }
                  }
               }
               drawTextureShader(shader, reflected, viewMatrix, sunDir, sunIntensity,
                  lightning,
                  deferred_diffuse_fbo_
                  );
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            break;

         case ShaderType::SKYBOX:
            if(printCurrentShaderName)
               printf("Skybox\n");
            shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);

            for (auto& drawable : culledDrawables) {
               if (drawable.draw_template.shader_type == ShaderType::SKYBOX) {
               /* doesn't use drawModelTransforms because no normals */
               for(const auto& mt : drawable.model_transforms) {
                  shader.sendUniform(Uniform::TEXTURE, uniforms, 
                        (*drawable.draw_template.texture).texture_slot());
                  (*drawable.draw_template.texture).enable(texture_cache_);
                  shader.sendUniform(Uniform::MODEL_VIEW, uniforms, viewMatrix * mt.model);
                  shader.drawMesh(drawable.draw_template.mesh);
               }
               }
            }
            break;

         case ShaderType::DEF_DIFFUSE:
            if(setOutputType < 0) {
               setOutputType = 0;
               deferred_diffuse_fbo_.bind();
            }
         case ShaderType::DEF_POSITION:
            if(setOutputType < 0) {
               setOutputType = 1;
               deferred_position_fbo_.bind();
            }
         case ShaderType::DEF_NORMAL:
            if(setOutputType < 0) {
               setOutputType = 2;
               deferred_normal_fbo_.bind();
            }
         case ShaderType::DEFERRED:
            if(printCurrentShaderName)
               printf("Deferred\n");

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
            shader.sendUniform(Uniform::OUTPUT_SHADER_TYPE, uniforms, setOutputType); 

            for (auto& drawable : culledDrawables) {
               Drawable newDrawable = Drawable::fromCulledDrawable(drawable, CullType::VIEW_CULLING);

               if(newDrawable.draw_template.shader_type == ShaderType::DEFERRED) {
                  newDrawable.draw_template.mesh.material.sendMaterial(shader, uniforms);
                  SendHeightMap(shader, newDrawable);
                  SendBones(shader, newDrawable);
                  SendTexture(shader, newDrawable);
                  drawModelTransforms(shader, newDrawable, viewMatrix, false);
               }
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            break;

         case ShaderType::TEXTURE:
            if(printCurrentShaderName)
               printf("Texture\n");
            {
               std::vector<Drawable> drawables;
               for (auto& drawable : culledDrawables) {
                  drawables.push_back(Drawable::fromCulledDrawable(drawable, CullType::VIEW_CULLING));
               }
               shader.sendUniform(Uniform::USE_BLINN_PHONG, uniforms, useBlinnPhong);
               SendShadow(shader, uniforms, shadow_map_fbo_, deerPos, sunDir);

               drawTextureShader(shader, drawables, viewMatrix, sunDir, sunIntensity, 
                  lightning, deferred_diffuse_fbo_);
            }
            break;

         case ShaderType::WATER:
            if(printCurrentShaderName)
               printf("Water\n");
            shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
            shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);
            shader.sendUniform(Uniform::SCREEN_WIDTH, uniforms, kScreenWidthf);
            shader.sendUniform(Uniform::SCREEN_HEIGHT, uniforms, kScreenHeightf);
            for (auto& drawable : culledDrawables) {
               Drawable newDrawable;
               newDrawable.draw_template = drawable.draw_template;
               if (drawable.draw_template.shader_type == ShaderType::WATER) {
                  SendTexture(shader, Drawable::fromCulledDrawable(drawable, CullType::REFLECT_CULLING));
                  drawModelTransforms(shader, newDrawable, viewMatrix, true);
               }
            }
            break;
         case ShaderType::FINAL_LIGHT_PASS:
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            SendSun(shader, uniforms, sunIntensity, sunDir);
            shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);

            for (auto& drawable : culledDrawables) {
               Drawable newDrawable = Drawable::fromCulledDrawable(drawable, CullType::VIEW_CULLING);

               if(newDrawable.draw_template.shader_type == ShaderType::FINAL_LIGHT_PASS) {
                  for(const auto& mt : drawable.model_transforms) {
                  shader.sendUniform(Uniform::MODEL_VIEW, uniforms, viewMatrix * mt.model);
                  shader.drawMesh(drawable.draw_template.mesh);
                  }
               }
            }
         break;
      }
   }
}

