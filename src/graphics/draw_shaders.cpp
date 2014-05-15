#include "draw_shaders.h"
#include "shaders.h"
#include "uniforms.h"
#include "shader_setup.h"

using namespace std;

void DrawShader::Draw(glm::mat4 viewMatrix, vector<Drawable> drawables) {
   for(auto& currentShader : shaders.getMap()) {
       for(auto& currentDraw : drawables) {
          Shader& shader = currentShader.second;
          if(currentDraw.draw_template.shaderType == currentShader.first) {
            setupTextureShader(shader, uniforms, currentDraw.draw_template.texture);
            shader.sendUniform(Uniform::HAS_BONES, uniforms, 1);
            shader.sendUniform(Uniform::BONES, uniforms,
            currentDraw.draw_template.mesh.animation.calculateBoneTransformations(currentDraw.draw_template.mesh.bone_array));
            
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

