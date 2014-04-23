#include "graphics/shaders.h"

#include <assert.h>

const std::vector<Attribute> kSunAttrs{
   Attribute::VERTEX,
   Attribute::NORMAL
};
const std::vector<Uniform> kSunUniforms{
   Uniform::MODEL,
   Uniform::VIEW,
   Uniform::PROJECTION,
   Uniform::NORMAL,
   Uniform::M_AMB,
   Uniform::M_DIF,
   Uniform::M_SPEC,
   Uniform::M_SHINE,
   Uniform::SUN_DIR,
   Uniform::SUN_INTENSITY
};

const std::vector<Attribute> kTextureAttrs{
   Attribute::VERTEX,
   Attribute::TEX_COORD,
// Attribute::NORMAL,
};
const std::vector<Uniform> kTextureUniforms{
   Uniform::MODEL,
   Uniform::VIEW,
   Uniform::PROJECTION,
   Uniform::TEXTURE
};

const std::vector<Attribute> kWireframeAttrs{
   Attribute::VERTEX
};
const std::vector<Uniform> kWireframeUniforms{
   Uniform::MODEL,
   Uniform::VIEW,
   Uniform::PROJECTION,
   Uniform::COLOR,
};

Shaders::Shaders() {
   shaders_.insert(std::make_pair(
            ShaderType::SUN,
            Shader("Sun", kSunAttrs, kSunUniforms)));
   shaders_.insert(std::make_pair(
            ShaderType::TEXTURE,
            Shader("Texture", kTextureAttrs, kTextureUniforms)));
   /*
   shaders_.insert(std::make_pair(
            ShaderType::WIREFRAME,
            Shader("wireframe", kWireframeAttrs, kWireframeUniforms)));
   */
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

UniformLocationMap Shaders::getUniformLocationMap() {
   UniformLocationMap uniforms;
   for (int i = static_cast<int>(Uniform::FIRST_UNIFORM);
        i < static_cast<int>(Uniform::LAST_UNIFORM);
        ++i) {
      const auto uniform = static_cast<Uniform>(i);
      for (auto& pair : shaders_) {
         const auto maybe_location = pair.second.uniformLocation(uniform);
         if (maybe_location)
            uniforms[uniform].insert(*maybe_location);
      }
   }
   return uniforms;
}

AttributeLocationMap Shaders::getAttributeLocationMap() {
   AttributeLocationMap attributes;
   for (int i = static_cast<int>(Attribute::FIRST_ATTRIBUTE);
        i < static_cast<int>(Attribute::LAST_ATTRIBUTE);
        ++i) {
      const auto attribute = static_cast<Attribute>(i);
      for (auto& pair : shaders_) {
         const auto maybe_location = pair.second.attributeLocation(attribute);
         if (maybe_location)
            attributes[attribute].insert(*maybe_location);
      }
   }
   return attributes;
}
