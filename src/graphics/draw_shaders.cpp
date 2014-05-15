#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include "shader_setup.h"

using namespace std;

void DrawShader::Draw(glm::mat4 viewMatrix, vector<Drawable> drawables) {
   for(auto& currentShader : shaders.getMap()) {
       for(auto& currentDraw : drawables) {
          Shader& shader = currentShader.second;

          /* instead of 0, put shadow_map_fbo.textureID() */
          shader.sendUniform(Uniform::SHADOW_MAP_TEXTURE, uniforms, 0);
          
          shader.sendUniform(Uniform::PROJECTION, uniforms, projectionMatrix);
          shader.sendUniform(Uniform::VIEW, uniforms, viewMatrix);

          shader.sendUniform(Uniform::LIGHTNING, uniforms, 0);

          shader.sendUniform(Uniform::HAS_SHADOWS, uniforms, 0);
          sendShadowInverseProjectionView(shader,uniforms, glm::vec3(0, 1, 0));
          shader.sendUniform(Uniform::HAS_HEIGHT_MAP, uniforms, 0); 
          
          if(currentDraw.draw_template.shaderType == currentShader.first) {
            setupTextureShader(shader, uniforms, currentDraw.draw_template.texture);
            shader.sendUniform(Uniform::HAS_BONES, uniforms, 1);
            shader.sendUniform(Uniform::BONES, uniforms,

            currentDraw.draw_template.mesh.animation.calculateBoneTransformations(
               currentDraw.draw_template.mesh.bone_array));
            
            for(auto& mt : currentDraw.model_transforms) {
               setupModelView(shader, uniforms, mt, viewMatrix, true);
               shader.drawMesh(currentDraw.draw_template.mesh);
            }

            shader.sendUniform(Uniform::HAS_BONES, uniforms, 0);
            currentDraw.draw_template.texture.disable();
         }
      }
   }   
}

