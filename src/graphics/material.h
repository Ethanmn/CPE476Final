#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>
#include "location_maps.h"
#include "uniforms.h"

struct Shader;
struct Shaders;

struct Material {
   glm::vec3 ambient;
   glm::vec3 diffuse;
   glm::vec3 specular;
   float shine;
};

void sendMaterial(Shader& shader, const UniformLocationMap& uniform_locations,
   glm::vec3 diffuse);

#endif
