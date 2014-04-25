#include "shaderSetup.h"
#include "shaders.h"
#include "shader.h"
#include "uniforms.h"
#include <glm/gtc/matrix_transform.hpp>

namespace  {
   const glm::mat4 projection_matrix = glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 500.f);
}


void setupModelView(Shader& shader, const UniformLocationMap& locations,
                    const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
                    bool needsNormal) {
   glPolygonMode(GL_FRONT, GL_FILL);
   shader.sendUniform(Uniform::MODEL_VIEW, locations, viewMatrix * modelMatrix);
   if(needsNormal) {
      shader.sendUniform(Uniform::NORMAL, locations,
         glm::transpose(glm::inverse(viewMatrix * modelMatrix)));
   }
}

void setupProjection(Shader& shader, const UniformLocationMap& locations) {
   glPolygonMode(GL_FRONT, GL_FILL);
   shader.sendUniform(Uniform::PROJECTION, locations, projection_matrix);
}

void setupView(Shader& shader, const UniformLocationMap& locations,
                            const glm::mat4& viewMatrix) {
   glPolygonMode(GL_FRONT, GL_FILL);
   shader.sendUniform(Uniform::VIEW, locations, viewMatrix);
}

void setupSunShader(Shader& shader, const UniformLocationMap& locations,
                    float sunIntensity, glm::vec3 sunDir) {
   glPolygonMode(GL_FRONT, GL_FILL);
   shader.sendUniform(Uniform::SUN_INTENSITY, locations, sunIntensity);
   shader.sendUniform(Uniform::SUN_DIR, locations, sunDir);
}

void setupTextureShader(Shader& shader, const UniformLocationMap& locations,
      float sunIntensity, int texture_id) {
   glPolygonMode(GL_FRONT, GL_FILL);
   shader.sendUniform(Uniform::TEXTURE, locations, texture_id);
   shader.sendUniform(Uniform::SUN_INTENSITY, locations, sunIntensity);
   
}

void setupWireframeShader(Shader& shader, const UniformLocationMap& locations,
                          glm::vec4 color) {
   glPolygonMode(GL_FRONT, GL_LINE);
   glLineWidth(1.0f);
   shader.sendUniform(Uniform::COLOR, locations, color);
}