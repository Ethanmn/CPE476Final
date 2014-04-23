#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>
#include "location_maps.h"
#include "uniforms.h"

struct Shader;
struct Shaders;

struct Material {
   Material();
   void sendToShader(Shader& shader, const UniformLocationMap& uniform_locations);
   void changeDiffuse(glm::vec3 diff, Shader& shader, const UniformLocationMap& uniform_locations);
   glm::vec3 ambient;
   glm::vec3 diffuse;
   glm::vec3 specular;
   float shine;
};

#endif