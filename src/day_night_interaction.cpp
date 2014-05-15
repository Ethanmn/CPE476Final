#include "day_night_interaction.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/shader_setup.h"
#include "graphics/material.h"

void DayNightInteraction::drawStart(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupModelView(shader, uniform_locations, start_transform_, viewMatrix, true);
   setupTextureShader(shader, uniform_locations, moon_texture_);
   shader.drawMesh(mesh_);
   moon_texture_.disable();
}
void DayNightInteraction::drawStop(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupModelView(shader, uniform_locations, stop_transform_, viewMatrix, true);
   setupTextureShader(shader, uniform_locations, sun_texture_);
   shader.drawMesh(mesh_);
   sun_texture_.disable();
}

void DayNightInteraction::shadowDrawGreen(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, bool betterShadow) {
   glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(-30.0, 2.0, -30.0));
   setupShadowShader(shader, uniform_locations, sunDir, model_matrix);
   shader.drawMesh(mesh_);
}

void DayNightInteraction::shadowDrawRed(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, bool betterShadow) {
   glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(20.0, 2.0, 20.0));
   setupShadowShader(shader, uniform_locations, sunDir, model_matrix);
   shader.drawMesh(mesh_);
}
