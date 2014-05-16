#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include "shader_setup.h"

using namespace std;

void DrawShader::Draw(glm::vec3 sunDir, float sunIntensity, glm::mat4 viewMatrix, 
      vector<Drawable> drawables) {
   for(auto& currentShader : shaders.getMap()) {
       for(auto& currentDraw : drawables) {
          Shader& shader = currentShader.second;
          shader.use();
          /* instead of 0, put shadow_map_fbo.textureID() */
          shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniforms, 0);
          shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
          shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);

          shader.sendUniform(Uniform::LIGHTNING, uniforms, 0);
          shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 0);
          sendShadowInverseProjectionView(shader,uniforms, glm::vec3(0, 1, 0));
          setupSunShader(shader, uniforms, sunIntensity, sunDir);       
          
          if(currentDraw.draw_template.shader_type == currentShader.first) {
             if(currentShader.first == ShaderType::TEXTURE) {
                 shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniforms, 0);
                 shader.sendUniform(Uniform::HAS_BONES, uniforms, 1);
                 shader.sendUniform(Uniform::BONES, uniforms,
                    currentDraw.draw_template.mesh.animation.calculateBoneTransformations(
                    currentDraw.draw_template.mesh.bone_array));
                 assert(currentDraw.draw_template.texture);
                 setupTextureShader(shader, uniforms, *currentDraw.draw_template.texture);
             }
             if(currentShader.first == ShaderType::SUN)
                 currentDraw.draw_template.mesh.material.sendMaterial(shader, uniforms);
            
            for(auto& mt : currentDraw.model_transforms) {
               setupModelView(shader, uniforms, mt, viewMatrix, true);
               shader.drawMesh(currentDraw.draw_template.mesh);
            }

            if(currentShader.first == ShaderType::TEXTURE) {
               shader.sendUniform(Uniform::HAS_BONES, uniforms, 0);
               currentDraw.draw_template.texture->disable();
            }
         }
      }
   }   
}
