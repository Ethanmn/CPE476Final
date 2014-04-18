#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>

struct Material {
   glm::vec3 ambient;
   glm::vec3 diffuse;
   glm::vec3 specular;
   float shine;
};

#endif