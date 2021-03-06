#ifndef SHADERS_H_
#define SHADERS_H_

#include <GL/glew.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "graphics/shader.h"
#include "graphics/location_maps.h"

enum class ShaderType {
   /* 
    * Ordering is important, shadow must be first.
    * Deferred must always be before Final.
    * Skybox is last so it renders after Final.
    * */
   SHADOW,
   /* Reflection generates a texture, so must be before TEXTURE */
   DEFERRED,
   REFLECTION,
   TEXTURE,

   FINAL_LIGHT_PASS,
   SKYBOX
};

struct Shaders {
   Shaders();

   Shader& at(ShaderType shader_type);
   void clear() { glUseProgram(0); }

   std::map<ShaderType, Shader>& getMap() { return shaders_; }
   UniformLocationMap getUniformLocationMap();
   AttributeLocationMap getAttributeLocationMap();

  private:
   std::map<ShaderType, Shader> shaders_;
};

#endif // SHADERS_H_
