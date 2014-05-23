#include "shader_setup.h"

#include <glm/gtc/matrix_transform.hpp>

#include "shaders.h"
#include "shader.h"
#include "texture.h"
#include "uniforms.h"

#define ORTHO_PROJ_AMOUNT 70.0f

namespace  {
   const glm::mat4 projection_matrix = glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 500.f);
   const glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 0.5, 0.0,
                              0.5, 0.5, 0.5, 1.0);
}

void setupModelView(Shader& shader, const UniformLocationMap& locations,
      const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, bool needsNormal) {
   glPolygonMode(GL_FRONT, GL_FILL);
   shader.sendUniform(Uniform::MODEL_VIEW, locations, viewMatrix * modelMatrix);
   if(needsNormal) {
      shader.sendUniform(Uniform::NORMAL, locations,
         glm::transpose(glm::inverse(viewMatrix * modelMatrix)));
   }
   shader.sendUniform(Uniform::MODEL, locations, modelMatrix);
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

void setupTextureShader(Shader& shader, const UniformLocationMap& locations, const Texture& texture) {
   shader.sendUniform(Uniform::HAS_TEXTURE, locations, 1);
   shader.sendUniform(Uniform::TEXTURE, locations, texture.textureID());
   texture.enable();
}

void setupHeightMap(Shader& shader, const UniformLocationMap& locations, const Texture& height_map) {
   shader.sendUniform(Uniform::HEIGHT_MAP, locations, height_map.textureID());
   shader.sendUniform(Uniform::HAS_HEIGHT_MAP, locations, 1);
   height_map.enable();
}

void setupWireframeShader(Shader& shader, const UniformLocationMap& locations,
      glm::vec4 color) {
   glPolygonMode(GL_FRONT, GL_LINE);
   glLineWidth(1.0f);
   shader.sendUniform(Uniform::COLOR, locations, color);
}


void setupShadowShader(Shader& shader, const UniformLocationMap& locations,
      glm::vec3 lightDir, glm::vec3 deerPos, glm::mat4 modelMatrix) {
   glPolygonMode(GL_FRONT, GL_FILL);
   glm::mat4 shadowProjection, shadowView, modelView;

   shadowProjection = glm::ortho(-ORTHO_PROJ_AMOUNT, ORTHO_PROJ_AMOUNT, 
                                 -ORTHO_PROJ_AMOUNT, ORTHO_PROJ_AMOUNT,
                                 -40.0f, 40.0f);
   shadowView = glm::lookAt(lightDir + deerPos, deerPos, glm::vec3(0.0, 1.0, 0.0));
   modelView = shadowView * modelMatrix;

   shader.sendUniform(Uniform::MODEL_VIEW, locations, modelView);
   shader.sendUniform(Uniform::PROJECTION, locations, shadowProjection);
}

void sendInverseViewProjection(Shader& shader, const UniformLocationMap& locations) {
   glPolygonMode(GL_FRONT, GL_FILL);
   shader.sendUniform(Uniform::PROJECTION, locations, projection_matrix);
}

void sendShadowInverseProjectionView(Shader& shader, const UniformLocationMap& locations,
      glm::vec3 lightDir, glm::vec3 deerPos) {
   glm::mat4 lightMat, shadowProjection, shadowView;

   shadowProjection = biasMatrix * glm::ortho(-ORTHO_PROJ_AMOUNT, ORTHO_PROJ_AMOUNT, 
                                              -ORTHO_PROJ_AMOUNT, ORTHO_PROJ_AMOUNT,
                                              -40.0f, 40.0f);
   shadowView = glm::lookAt(lightDir + deerPos, deerPos, glm::vec3(0.0, 1.0, 0.0));
   lightMat = shadowProjection * shadowView;

   shader.sendUniform(Uniform::SHADOW_MAP, locations, lightMat);
}

