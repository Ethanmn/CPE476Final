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
}
void DayNightInteraction::drawStop(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupModelView(shader, uniform_locations, stop_transform_, viewMatrix, true);
   setupTextureShader(shader, uniform_locations, sun_texture_);
   shader.drawMesh(mesh_);
}
