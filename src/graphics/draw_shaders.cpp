#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include <iostream>
#include <SDL.h>

#include "frustum.h"

using namespace std;
namespace {
   enum DeferredType {
      DEFERRED_DIFFUSE,
      DEFERRED_POSITION,
      DEFERRED_NORMAL,
      LAST_DEFERRED,
   };

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

   void SendModelAndNormal(Shader& shader, const UniformLocationMap& uniforms,
         const glm::mat4& model_view, const glm::mat4& model, bool needsModel) {
      shader.sendUniform(Uniform::NORMAL, uniforms, glm::transpose(glm::inverse(model_view)));
      if (needsModel)
         shader.sendUniform(Uniform::MODEL, uniforms, model);
   }
}

void DrawShader::drawModelTransforms(Shader& shader, const Drawable& drawable,
      const glm::mat4& view, bool needsModel, const UniformLocationMap& uniforms) {
   for(const auto& instance : drawable.draw_instances) {
      SendModelAndNormal(shader, uniforms, view*instance.model_transform, instance.model_transform, needsModel);
      shader.drawMesh(drawable.draw_template.mesh);
   }
}

void DrawShader::drawModelTransforms(
      Shader& shader,
      const Drawable& drawable,
      const glm::mat4& view,
      const FrameBufferObject& deferred_diffuse_fbo,
      const FrameBufferObject& deferred_position_fbo,
      const FrameBufferObject& deferred_normal_fbo,
      bool needsModel,
      const UniformLocationMap& uniforms) {
   for(const auto& instance : drawable.draw_instances) {
      if (instance.material) {
         instance.material->sendMaterial(shader, uniforms);
      }

      const auto model_view = view * instance.model_transform;
      shader.sendUniform(Uniform::MODEL_VIEW_PROJECTION, uniforms, gProjectionMatrix * model_view);
      SendModelAndNormal(shader, uniforms, model_view, instance.model_transform, needsModel);
      for (int i = 0; i < LAST_DEFERRED; ++i) {
         const auto deferred_type(static_cast<DeferredType>(i));
         switch (deferred_type) {
            case DEFERRED_DIFFUSE:
               deferred_diffuse_fbo.bind();
               break;
            case DEFERRED_POSITION:
               deferred_position_fbo.bind();
               break;
            case DEFERRED_NORMAL:
               deferred_normal_fbo.bind();
               break;
            default: break;
         }
         shader.sendUniform(Uniform::OUTPUT_SHADER_TYPE, uniforms, i);
         shader.drawMesh(drawable.draw_template.mesh);
      }
   }
}

void DrawShader::drawTextureShader(Shader& shader, const std::vector<Drawable>& drawables,
      const glm::mat4& viewMatrix, const glm::vec3& sunDir, float sunIntensity, 
      int lightning) {

   shader.sendUniform(Uniform::PROJECTION, uniforms, gProjectionMatrix);
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

            drawable.draw_template.material.sendMaterial(shader, uniforms);
         }
         drawModelTransforms(shader, drawable, viewMatrix, true, uniforms);
      }
   }
}


void DrawShader::SendHeightMap(Shader& shader, const Drawable& drawable) {
   if (drawable.draw_template.height_map) {
      shader.sendUniform(Uniform::HEIGHT_MAP, uniforms,
            (*drawable.draw_template.height_map).texture_slot());
      shader.sendUniform(Uniform::HEIGHT_MAP_SCALE, uniforms, gHeightMapScale);
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
                  glPolygonMode(GL_FRONT, GL_FILL);
                  const auto drawables = Drawable::fromCulledDrawables(shadow_drawables, CullType::VIEW_CULLING);
                  for (auto& drawable : drawables) {
                     SendBones(shader, drawable);
                     if (drawable.draw_template.effects.count(EffectType::CASTS_SHADOW)) {
                        SendTexture(shader, drawable);
                        for(auto& instance : drawable.draw_instances) {
                           shader.sendUniform(Uniform::MODEL_VIEW_PROJECTION, uniforms,
                                 view_projection * instance.model_transform);
                           shader.sendUniform(Uniform::MODEL, uniforms, instance.model_transform);
                           shader.drawMesh(drawable.draw_template.mesh);
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
            {
               reflection_fbo.bind();
               // Cheap hack: just use view culling.
               auto drawables = Drawable::fromCulledDrawables(culledDrawables, CullType::VIEW_CULLING);
               // scale everything across the ground.
               const auto scale = glm::scale(glm::mat4(), glm::vec3(1, -1, 1));
               for (std::vector<Drawable>::iterator iter = drawables.begin(); iter != drawables.end();) {
                  // Cheap hack to get rid of the ground plane. DEADLINE
                  // APPROACHES.
                  if (iter->draw_template.height_map) {
                     iter = drawables.erase(iter);
                  } else {
                     for (auto& instance : iter->draw_instances) {
                        instance.model_transform = scale * instance.model_transform;
                     }
                     ++iter;
                  }
               }
               const auto reflectSunDir = glm::vec3(scale * glm::vec4(sunDir, 0));

               shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 0);
               shader.sendUniform(Uniform::USE_BLINN_PHONG, uniforms, useBlinnPhong);

               drawTextureShader(shader, drawables, viewMatrix, reflectSunDir, sunIntensity,
                     lightning);
            }
            break;
         case ShaderType::WATER:
            break;
         case ShaderType::TEXTURE:
            if (!useTextureShader) break;
            if(printCurrentShaderName)
               printf("Texture\n");

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            {
               const auto drawables = Drawable::fromCulledDrawables(culledDrawables, CullType::VIEW_CULLING);
               shader.sendUniform(Uniform::USE_BLINN_PHONG, uniforms, useBlinnPhong);
               shadow_map_fbo_.texture().enable(texture_cache_);
               SendShadow(shader, uniforms, shadow_map_fbo_, deerPos, sunDir);

               drawTextureShader(shader, drawables, viewMatrix, sunDir, sunIntensity,
                     lightning);
            }
            break;

         case ShaderType::SKYBOX:
            if(printCurrentShaderName)
               printf("Skybox\n");
            shader.sendUniform(Uniform::PROJECTION, uniforms, gProjectionMatrix);

            for (auto& drawable : culledDrawables) {
               if (drawable.draw_template.shader_type == ShaderType::SKYBOX) {
                  /* doesn't use drawModelTransforms because no normals */
                  for(const auto& instance : drawable.draw_instances) {
                     shader.sendUniform(Uniform::TEXTURE, uniforms,
                           (*drawable.draw_template.texture).texture_slot());
                     (*drawable.draw_template.texture).enable(texture_cache_);
                     shader.sendUniform(Uniform::MODEL_VIEW, uniforms, viewMatrix * instance.instance.model_transform);
                     shader.drawMesh(drawable.draw_template.mesh);
                  }
               }
            }
            break;

         case ShaderType::DEFERRED:
            if (useTextureShader) break;
            if(printCurrentShaderName)
               printf("Deferred\n");
            for (size_t i = 0; i < LAST_DEFERRED; ++i) {
               DeferredType deferred_type = static_cast<DeferredType>(i);
               switch (deferred_type) {
                  case DEFERRED_DIFFUSE:
                     deferred_diffuse_fbo_.bind();
                     break;
                  case DEFERRED_POSITION:
                     deferred_position_fbo_.bind();
                     break;
                  case DEFERRED_NORMAL:
                     deferred_normal_fbo_.bind();
                     break;
                  default: break;
               }
               glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            shadow_map_fbo_.texture().enable(texture_cache_);
            SendShadow(shader, uniforms, shadow_map_fbo_, deerPos, sunDir);

            for (auto& drawable : culledDrawables) {
               Drawable newDrawable = Drawable::fromCulledDrawable(drawable, CullType::VIEW_CULLING);
               if (newDrawable.draw_template.shader_type == ShaderType::DEFERRED) {
                  newDrawable.draw_template.material.sendMaterial(shader, uniforms);
                  SendHeightMap(shader, newDrawable);
                  SendBones(shader, newDrawable);
                  SendTexture(shader, newDrawable);
                  {
                     const int vary_material = drawable.draw_template.effects.count(EffectType::VARY_MATERIAL);
                     shader.sendUniform(Uniform::VARY_MATERIAL, uniforms, vary_material);
                  }
                  drawModelTransforms(shader, newDrawable, viewMatrix,
                        deferred_diffuse_fbo_,
                        deferred_position_fbo_,
                        deferred_normal_fbo_,
                        true, uniforms);
               }
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            break;

         case ShaderType::FINAL_LIGHT_PASS:
            if (useTextureShader) break;
            glm::mat4 curView;
            glm::mat4 lookAt = glm::lookAt( glm::vec3(2.0f, 0.f,0.0f),glm::vec3(0.f, 0.f, 0.f),glm::vec3( 0.0f, 1.0f, 0.0f ) );
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            deferred_diffuse_fbo_.texture().enable(texture_cache_);
            shader.sendUniform(Uniform::FINAL_PASS_DIFFUSE_TEXTURE, uniforms, deferred_diffuse_fbo_.texture_slot());
            deferred_position_fbo_.texture().enable(texture_cache_);
            shader.sendUniform(Uniform::FINAL_PASS_POSITION_TEXTURE, uniforms, deferred_position_fbo_.texture_slot());
            deferred_normal_fbo_.texture().enable(texture_cache_);
            shader.sendUniform(Uniform::FINAL_PASS_NORMAL_TEXTURE, uniforms, deferred_normal_fbo_.texture_slot());

            SendSun(shader, uniforms, sunIntensity, sunDir);
            shader.sendUniform(Uniform::PROJECTION, uniforms, gProjectionMatrix);
            shader.sendUniform(Uniform::SCREEN_WIDTH, uniforms, kScreenWidthf);
            shader.sendUniform(Uniform::SCREEN_HEIGHT, uniforms, kScreenHeightf);
            shader.sendUniform(Uniform::LIGHTNING, uniforms, lightning);

            for (auto& drawable : culledDrawables) {
               Drawable newDrawable = Drawable::fromCulledDrawable(drawable, CullType::VIEW_CULLING);

               if(newDrawable.draw_template.shader_type == ShaderType::FINAL_LIGHT_PASS) {

                  shader.sendUniform(Uniform::IS_FIREFLY, uniforms, 0);
                  if (drawable.draw_template.effects.count(EffectType::IS_GOD_RAY)) {
                     shader.sendUniform(Uniform::IS_GOD_RAY, uniforms, 1);
                     if(drawable.draw_template.effects.count(EffectType::IS_FIREFLY))
                        shader.sendUniform(Uniform::IS_FIREFLY, uniforms, 1);
                     curView = viewMatrix;
                  }
                  else {
                     shader.sendUniform(Uniform::IS_GOD_RAY, uniforms, 0);
                     curView = lookAt;
                  }

                  for(const auto& instance : drawable.draw_instances) {
                     if (drawable.draw_template.effects.count(EffectType::IS_GOD_RAY)) {
                        glm::vec3 ray_pos = glm::vec3(instance.instance.model_transform * glm::vec4(0,0,0,1));
                        shader.sendUniform(Uniform::GOD_RAY_CENTER, uniforms, ray_pos);
                     }
                     shader.sendUniform(Uniform::MODEL, uniforms, instance.instance.model_transform);
                     shader.sendUniform(Uniform::VIEW, uniforms, curView);
                     shader.sendUniform(Uniform::NORMAL, uniforms,
                           glm::transpose(glm::inverse(curView * instance.instance.model_transform)));
                     shader.drawMesh(drawable.draw_template.mesh);
                  }
               }
            }
            break;
      }
   }
}
