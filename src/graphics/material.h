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
   Material(const glm::vec3& diff = glm::vec3(1.0),
            const glm::vec3& amb = glm::vec3(0.1),
            const glm::vec3& spec = glm::vec3(0.1),
            float shineVal = 1000.0f);
   void sendMaterial(Shader& shader, const UniformLocationMap& uniform_locations) const;
};

#endif
