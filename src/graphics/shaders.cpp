#include "graphics/shaders.h"
#include <assert.h>

namespace {
   bool runDeferred = true;
}

const std::vector<Attribute> kTextureAttrs{
   Attribute::VERTEX,
   Attribute::TEX_COORD,
   Attribute::NORMAL,

   Attribute::BONE_ID0,
   Attribute::BONE_WEIGHT0,

   Attribute::BONE_ID1,
   Attribute::BONE_WEIGHT1,

   Attribute::BONE_ID2,
   Attribute::BONE_WEIGHT2,

   Attribute::BONE_ID3,
   Attribute::BONE_WEIGHT3,

   Attribute::BONE_ID4,
   Attribute::BONE_WEIGHT4,
};
const std::vector<Uniform> kTextureUniforms{
   Uniform::MODEL_VIEW,
   Uniform::MODEL,
   Uniform::VIEW,
   Uniform::PROJECTION,
   Uniform::NORMAL,

   Uniform::M_AMB,
   Uniform::M_DIF,
   Uniform::M_SPEC,
   Uniform::M_SHINE,
   
   Uniform::HAS_SHADOWS,
   Uniform::SHADOW_MAP,
   Uniform::SHADOW_MAP_TEXTURE,
   
   Uniform::HAS_TEXTURE,
   Uniform::TEXTURE,
   
   Uniform::HEIGHT_MAP,
   Uniform::HEIGHT_MAP_SCALE,
   Uniform::HAS_HEIGHT_MAP,
   
   Uniform::USE_BLINN_PHONG,
   Uniform::SUN_DIR,
   Uniform::SUN_INTENSITY,
   
   Uniform::BONES,
   Uniform::HAS_BONES,
  
   Uniform::LIGHTNING,

   //Uniform::GOD_RAY_POSITION,
   //Uniform::GOD_RAY_RADIUS
};

/* Deferred Pass Attributes are the same as Texture's */
const std::vector<Uniform> kDeferredPassUniforms {
   Uniform::MODEL_VIEW,
   Uniform::MODEL,
   Uniform::PROJECTION,
   Uniform::NORMAL,

   Uniform::M_AMB,
   Uniform::M_DIF,
   Uniform::M_SPEC,
   Uniform::M_SHINE,
   
   Uniform::OUTPUT_SHADER_TYPE,
   
   Uniform::HAS_TEXTURE,
   Uniform::HEIGHT_MAP_SCALE,
   Uniform::TEXTURE,
   
   Uniform::HEIGHT_MAP,
   Uniform::HAS_HEIGHT_MAP,
      
   Uniform::BONES,
   Uniform::HAS_BONES,

   Uniform::HAS_SHADOWS,
   Uniform::SHADOW_MAP,
   Uniform::SHADOW_MAP_TEXTURE,
};

const std::vector<Attribute> kShadowAttrs{
   Attribute::VERTEX,
   Attribute::TEX_COORD,

   Attribute::BONE_ID0,
   Attribute::BONE_WEIGHT0,

   Attribute::BONE_ID1,
   Attribute::BONE_WEIGHT1,

   Attribute::BONE_ID2,
   Attribute::BONE_WEIGHT2,

   Attribute::BONE_ID3,
   Attribute::BONE_WEIGHT3,

   Attribute::BONE_ID4,
   Attribute::BONE_WEIGHT4,
};
const std::vector<Uniform> kShadowUniforms{
   Uniform::MODEL_VIEW_PROJECTION,
   Uniform::PROJECTION,

   Uniform::HAS_TEXTURE,
   Uniform::TEXTURE,
   
   Uniform::BONES,
   Uniform::HAS_BONES,
};

const std::vector<Attribute> kSkyboxAttrs{
   Attribute::VERTEX,
   Attribute::TEX_COORD
};
const std::vector<Uniform> kSkyboxUniforms{
   Uniform::MODEL_VIEW,
   Uniform::PROJECTION,
   Uniform::TEXTURE
};

const std::vector<Attribute> kWaterAttrs{
   Attribute::VERTEX,
};
const std::vector<Uniform> kWaterUniforms{
   Uniform::MODEL_VIEW,
   Uniform::PROJECTION,
   Uniform::TEXTURE,
   Uniform::SCREEN_WIDTH,
   Uniform::SCREEN_HEIGHT,
};

const std::vector<Attribute> kFinalPassAttrs{
   Attribute::VERTEX,
   Attribute::TEX_COORD
};
const std::vector<Uniform> kFinalPassUniforms{
   Uniform::MODEL,
   Uniform::VIEW,
   Uniform::PROJECTION,

   Uniform::IS_GOD_RAY,
   Uniform::GOD_RAY_CENTER,

   Uniform::SCREEN_WIDTH,
   Uniform::SCREEN_HEIGHT,
   
   Uniform::SUN_DIR,
   Uniform::SUN_INTENSITY,

   Uniform::FINAL_PASS_POSITION_TEXTURE,
   Uniform::FINAL_PASS_DIFFUSE_TEXTURE,
   Uniform::FINAL_PASS_NORMAL_TEXTURE,
};

Shaders::Shaders() {
      shaders_.insert(std::make_pair(ShaderType::SHADOW, 
            Shader("Shadow", kShadowAttrs, kShadowUniforms)));
      shaders_.insert(std::make_pair(ShaderType::SKYBOX,
            Shader("Skybox", kSkyboxAttrs, kSkyboxUniforms)));      
      shaders_.insert(std::make_pair(ShaderType::DEF_DIFFUSE,
            Shader("Deferred", kTextureAttrs, kDeferredPassUniforms)));
      shaders_.insert(std::make_pair(ShaderType::DEF_POSITION,
            Shader("Deferred", kTextureAttrs, kDeferredPassUniforms)));
      shaders_.insert(std::make_pair(ShaderType::DEF_NORMAL,
            Shader("Deferred", kTextureAttrs, kDeferredPassUniforms)));

   if(runDeferred) {
      shaders_.insert(std::make_pair(ShaderType::FINAL_LIGHT_PASS,
            Shader("Final", kFinalPassAttrs, kFinalPassUniforms)));
   }
   else {
      shaders_.insert(std::make_pair(ShaderType::TEXTURE,
            Shader("Texture", kTextureAttrs, kTextureUniforms)));
      shaders_.insert(std::make_pair(ShaderType::REFLECTION,
            Shader("Texture", kTextureAttrs, kTextureUniforms)));
      shaders_.insert(std::make_pair(ShaderType::WATER,
            Shader("Water", kWaterAttrs, kWaterUniforms)));
      shaders_.insert(std::make_pair(ShaderType::SKYBOX,
            Shader("Skybox", kSkyboxAttrs, kSkyboxUniforms)));
   }
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
