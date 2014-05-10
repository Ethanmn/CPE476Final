#include "day_night_interaction.h"
#include "graphics/shader.h"
#include "graphics/shaders.h"
#include "graphics/shader_setup.h"
#include "graphics/material.h"

void DayNightInteraction::drawStart(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupModelView(shader, uniform_locations, start_transform_, viewMatrix, true);
   mesh_.material = Material(glm::vec3(0.5f, 0.7f, 0.5f));
   mesh_.material.sendMaterial(shader, uniform_locations);
   shader.drawMesh(mesh_);
}
void DayNightInteraction::drawStop(Shader& shader, const UniformLocationMap& uniform_locations,
                       const glm::mat4& viewMatrix) {
   setupModelView(shader, uniform_locations, stop_transform_, viewMatrix, true);
   mesh_.material = Material(glm::vec3(0.7f, 0.5f, 0.5f));
   mesh_.material.sendMaterial(shader, uniform_locations);
   shader.drawMesh(mesh_);
}
