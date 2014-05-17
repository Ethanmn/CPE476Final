#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include "shader_setup.h"


using namespace std;
namespace {
   bool debug = false;
}

void DrawShader::Draw(ShadowMapFBO shadow_map_fbo_, vector<Drawable> drawables, glm::mat4 viewMatrix,
      glm::vec3 sunDir, float sunIntensity, int lightning) {
   for(auto& currentShader : shaders.getMap()) {
      for(auto& currentDraw : drawables) {
         Shader& shader = currentShader.second;
         shader.use();
         if(currentShader.first == ShaderType::SHADOW && currentDraw.draw_template.include_in_shadows) {
            if(!debug) {
               shadow_map_fbo_.BindForWriting();
               glClear(GL_DEPTH_BUFFER_BIT);
            }

            for(auto& mt : currentDraw.model_transforms) { 
               setupShadowShader(shader, uniforms, sunDir, mt); 
               shader.drawMesh(currentDraw.draw_template.mesh);
            }

            if(!debug) {
               glBindFramebuffer(GL_FRAMEBUFFER, 0);
               glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
               shadow_map_fbo_.BindForReading();
            } 
         }
         /* All shaders that are not SHADOW */
         else if(!debug && currentDraw.draw_template.shader_type == currentShader.first) {
            shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 1);
            shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniforms, shadow_map_fbo_.texture_id());
            sendShadowInverseProjectionView(shader,uniforms, sunDir);

            shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
            shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);

            shader.sendUniform(Uniform::LIGHTNING, uniforms, lightning);
            setupSunShader(shader, uniforms, sunIntensity, sunDir); 

            if(currentShader.first == ShaderType::TEXTURE) {
               if(currentDraw.draw_template.height_map) 
                  setupHeightMap(shader, uniforms, *currentDraw.draw_template.height_map);
               else
                  shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniforms, 0);

               if(currentDraw.draw_template.has_bones) {
                  shader.sendUniform(Uniform::HAS_BONES, uniforms, 1);
                  shader.sendUniform(Uniform::BONES, uniforms,
                        currentDraw.draw_template.mesh.animation.calculateBoneTransformations(
                           currentDraw.draw_template.mesh.bone_array));
               }
               else
                  shader.sendUniform(Uniform::HAS_BONES, uniforms, 0);

               assert(currentDraw.draw_template.texture);
               setupTextureShader(shader, uniforms, *currentDraw.draw_template.texture);
            }
            else if(currentShader.first == ShaderType::SUN)
               currentDraw.draw_template.mesh.material.sendMaterial(shader, uniforms);

            for(auto& mt : currentDraw.model_transforms) {
               setupModelView(shader, uniforms, mt, viewMatrix, true);
               shader.drawMesh(currentDraw.draw_template.mesh);
            }

            if(currentShader.first == ShaderType::TEXTURE) {
               shader.sendUniform(Uniform::HAS_BONES, uniforms, 0);
               currentDraw.draw_template.texture->disable();
            }

            if(currentDraw.draw_template.height_map) 
               currentDraw.draw_template.height_map->disable(); 
         }
      }

   }   
}
