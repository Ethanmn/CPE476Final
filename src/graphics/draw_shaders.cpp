#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include "shader_setup.h"


using namespace std;
namespace {
   bool debug = false;
}

// Setup common to both Texture/Sun Shaders
void DrawShader::setupDrawShader(Shader& shader, FrameBufferObject shadow_map_fbo_, glm::mat4 viewMatrix, glm::vec3 deerPos,
      glm::vec3 sunDir, float sunIntensity, int lightning) {
   shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 1);
   shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniforms, shadow_map_fbo_.texture_slot());
   sendShadowInverseProjectionView(shader, uniforms, sunDir, deerPos);

   shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
   shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);

   shader.sendUniform(Uniform::LIGHTNING, uniforms, lightning);
   setupSunShader(shader, uniforms, sunIntensity, sunDir); 
}

void DrawShader::Draw(FrameBufferObject shadow_map_fbo_, vector<Drawable> drawables, glm::mat4 viewMatrix, glm::vec3 deerPos,
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

            for (auto& drawable : drawables) {
               if (drawable.draw_template.effects.count(EffectType::CASTS_SHADOW)) {
                  for(auto& mt : drawable.model_transforms) {
                     setupShadowShader(shader, uniforms, sunDir, deerPos, mt);
                     shader.drawMesh(drawable.draw_template.mesh);
                  }
               }
            }

            if(!debug) {
               glBindFramebuffer(GL_FRAMEBUFFER, 0);
               glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
               shadow_map_fbo_.BindForReading();
            }
            break;
         case ShaderType::TEXTURE_BLINN: 
            /* 
             * Blinn-Phong handles all of the uniforms the same as texture, just
             * changes how the uniforms are used in the fragment shader 
             */
         case ShaderType::TEXTURE:
            setupDrawShader(shader, shadow_map_fbo_, viewMatrix, deerPos, sunDir, sunIntensity, lightning);
            for (auto& drawable : drawables) {
               if (drawable.draw_template.shader_type == shader_pair.first) {
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
            break;
      }
   }
}

void DrawShader::drawModelTransforms(Shader& shader, const Drawable& drawable, const glm::mat4& view) {
   for(const auto& mt : drawable.model_transforms) {
      setupModelView(shader, uniforms, mt, view, true);
      shader.drawMesh(drawable.draw_template.mesh);
   }
}
