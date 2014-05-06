#include "material.h"
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "shaders.h"

Material::Material(const glm::vec3& diff, const glm::vec3& amb,
         const glm::vec3& spec, float shineVal) {
   diffuse = diff;
   ambient = amb;
   specular = spec;
   shine = shineVal;
}

void Material::sendMaterial(Shader& shader, const UniformLocationMap& uniform_locations) const {
   shader.sendUniform(Uniform::M_AMB, uniform_locations, ambient);
   shader.sendUniform(Uniform::M_DIF, uniform_locations, diffuse);
   shader.sendUniform(Uniform::M_SPEC, uniform_locations, specular);
   shader.sendUniform(Uniform::M_SHINE, uniform_locations, shine);
}