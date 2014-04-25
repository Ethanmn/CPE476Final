#include "setupShader.cpp"

namespace {
   const glm::mat4 projectionMatrix = glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 500.f);
}

void setupModelView(Shader& shader, const UniformLocationMap& locations,
                    const glm::mat4& viewModelMatrix, bool needsNormal) const {
   shader.sendUniform(Uniform::MODEL_VIEW, uniform_location_map_, viewModelMatrix);
   if(needsNormal) {
      shader.sendUniform(Uniform::NORMAL, uniform_location_map_,
                         glm::transpose(glm::inverse(viewModelMatrix)));
}

void setupViewAndProjection(Shader& shader, const UniformLocationMap& locations,
                      const glm::mat4& viewMatrix) const {
   shader.sendUniform(Uniform::VIEW, uniform_location_map_, viewMatrix);
   shader.sendUniform(Uniform::PROJECTION, uniform_location_map_, projectionMatrix);
}


void setupSunShader(Shader& shader, const UniformLocationMap& locations,
                    float sunIntensity, glm::vec3 sunDir) const {
   shader.sendUniform(Uniform::SUN_INTENSITY, uniform_location_map_, sunIntensity);
   shader.sendUniform(Uniform::SUN_DIR, uniform_location_map_, sunDir);
}

void setupTextureShader(Shader& shader, const UniformLocationMap& locations,
      float sunIntensity) const {
   shader.sendUniform(Uniform::TEXTURE, uniform_location_map_, 0);
   shader.sendUniform(Uniform::SUN_INTENSITY, uniform_location_map_, sunIntensity);
   
}

void setupWireframeShader(Shader& shader, const UniformLocationMap& locations,
                          glm::vec4 color) const {
   shader.sendUniform(Uniform::COLOR, uniform_locations, color);
}