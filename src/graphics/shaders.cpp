#include "graphics/shaders.h"

#include <assert.h>

const std::vector<Attribute> kSunAttrs{
   Attribute::VERTEX,
   Attribute::NORMAL
};
const std::vector<Uniform> kSunUniforms{
   Uniform::VIEW,
   Uniform::PROJECTION,
   Uniform::SUNDIR,
   Uniform::SUNINTENSITY
};

Shaders::Shaders() {
   shaders_.insert(std::make_pair(
            ShaderType::SUN,
            Shader("Sun", kSunAttrs, kSunUniforms)));
}

Shader& Shaders::at(ShaderType shader_type) {
   if (shaders_.count(shader_type) > 0) {
      return shaders_.at(shader_type);
   } else {
      std::cerr << "Uninitialized ShaderType. ";
      std::cerr << "Perhaps you meant to initialize it in Shaders::Shaders()?" << std::endl;
      exit(EXIT_FAILURE);
   }
}

GLUniformLocationMap Shaders::getUniforms(const Uniform& uniform) {
   GLUniformLocationMap uniforms;
   for (auto& pair : shaders_) {
      const auto maybe_location = pair.second.uniformLocation(uniform);
      if (maybe_location)
         uniforms.insert(*maybe_location);
   }
   return uniforms;
}

GLAttributeLocationMap Shaders::getAttributes(const Attribute& attribute) {
   GLAttributeLocationMap attributes;
   for (auto& pair : shaders_) {
      const auto maybe_location = pair.second.attributeLocation(attribute);
      if (maybe_location)
         attributes.insert(*maybe_location);
   }
   return attributes;
}
