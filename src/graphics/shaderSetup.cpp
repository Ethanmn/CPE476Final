#include "shaderSetup.h"
#include "shaders.h"
#include "shader.h"
#include "uniforms.h"
#include <glm/gtc/matrix_transform.hpp>

namespace  {
   const glm::mat4 projection_matrix = glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 500.f);
}


void setupModelView(Shader& shader, const UniformLocationMap& locations,
                    const glm::mat4& viewModelMatrix, bool needsNormal) {
   shader.sendUniform(Uniform::MODEL_VIEW, locations, viewModelMatrix);
   if(needsNormal) {
      shader.sendUniform(Uniform::NORMAL, locations,
         glm::transpose(glm::inverse(viewModelMatrix)));
   }
}

void setupProjection(Shader& shader, const UniformLocationMap& locations) {
   shader.sendUniform(Uniform::PROJECTION, locations, projection_matrix);
}

void setupView(Shader& shader, const UniformLocationMap& locations,
                            const glm::mat4& viewMatrix) {
   shader.sendUniform(Uniform::VIEW, locations, viewMatrix);
}

void setupSunShader(Shader& shader, const UniformLocationMap& locations,
                    float sunIntensity, glm::vec3 sunDir) {
   shader.sendUniform(Uniform::SUN_INTENSITY, locations, sunIntensity);
   shader.sendUniform(Uniform::SUN_DIR, locations, sunDir);
}

void setupTextureShader(Shader& shader, const UniformLocationMap& locations,
      float sunIntensity) {
   shader.sendUniform(Uniform::TEXTURE, locations, 0);
   shader.sendUniform(Uniform::SUN_INTENSITY, locations, sunIntensity);
   
}

void setupWireframeShader(Shader& shader, const UniformLocationMap& locations,
                          glm::vec4 color) {
   glPolygonMode(GL_FRONT, GL_LINE);
   glLineWidth(1.0f);
   shader.sendUniform(Uniform::COLOR, locations, color);
}