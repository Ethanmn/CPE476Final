#include "shader_setup.h"

#include <glm/gtc/matrix_transform.hpp>

#include "shaders.h"
#include "shader.h"
#include "texture.h"
#include "uniforms.h"

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

void setupTextureShader(Shader& shader, const UniformLocationMap& locations, const Texture& texture) {
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
