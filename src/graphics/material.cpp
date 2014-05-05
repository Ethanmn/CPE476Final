#include "material.h"
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "shaders.h"

void sendMaterial(Shader& shader, const UniformLocationMap& uniform_locations, glm::vec3 diffuse) {
   shader.sendUniform(Uniform::M_AMB, uniform_locations, glm::vec3(0.1f, 0.1f, 0.1f));
   shader.sendUniform(Uniform::M_DIF, uniform_locations, diffuse);
   shader.sendUniform(Uniform::M_SPEC, uniform_locations, glm::vec3(0.1f, 0.0f, 0.1f));
   shader.sendUniform(Uniform::M_SHINE, uniform_locations, 100.0f);
}
