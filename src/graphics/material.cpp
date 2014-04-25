#include "material.h"
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "shaders.h"

Material::Material() :
   ambient(glm::vec3(0.1f, 0.1f, 0.1f)),
   diffuse(glm::vec3(0.7f, 0.5f, 0.7f)),
   specular(glm::vec3(0.1f, 0.0f, 0.1f)),
   shine(100.0f)
{}

void Material::changeDiffuse(glm::vec3 diff, Shader& shader, const UniformLocationMap& uniform_locations) {
   diffuse = diff;
   shader.sendUniform(Uniform::M_DIF, uniform_locations, diffuse);
}

void Material::sendToShader(Shader& shader, const UniformLocationMap& uniform_locations) {
   shader.sendUniform(Uniform::M_AMB, uniform_locations, ambient);
   shader.sendUniform(Uniform::M_DIF, uniform_locations, diffuse);
   shader.sendUniform(Uniform::M_SPEC, uniform_locations, specular);
   shader.sendUniform(Uniform::M_SHINE, uniform_locations, shine);
}

void sendMaterial(Shader& shader, const UniformLocationMap& uniform_locations,
                  glm::vec3 diffuse) {
   shader.sendUniform(Uniform::M_AMB, uniform_locations, glm::vec3(0.1f, 0.1f, 0.1f));
   shader.sendUniform(Uniform::M_DIF, uniform_locations, diffuse);
   shader.sendUniform(Uniform::M_SPEC, uniform_locations, glm::vec3(0.1f, 0.0f, 0.1f));
   shader.sendUniform(Uniform::M_SHINE, uniform_locations, 100.0);
}
