#ifndef SHADERS_H_
#define SHADERS_H_

#include <GL/glew.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "graphics/shader.h"
#include "graphics/uniform_location_map.h"

enum class ShaderType {
   SUN
};

struct Shaders {
   Shaders();

   Shader& at(ShaderType shader_type);
   void clear() { glUseProgram(0); }

   std::map<ShaderType, Shader>& getMap() { return shaders_; }
   UniformLocationMap getUniforms(const Uniform& uniform);
   GLAttributeLocationMap getAttributes(const Attribute& attribute);

  private:
   std::map<ShaderType, Shader> shaders_;
};

#endif // SHADERS_H_
