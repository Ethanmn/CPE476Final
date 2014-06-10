#ifndef UNIFORMS_H_
#define UNIFORMS_H_

#include <string>

enum class Uniform {
   FIRST_UNIFORM,
   MODEL_VIEW = FIRST_UNIFORM,
   MODEL_VIEW_PROJECTION,
   MODEL,
   VIEW,
   PROJECTION,
   NORMAL,
   TEXTURE,
   HAS_HEIGHT_MAP,
   HEIGHT_MAP_SCALE,
   HEIGHT_MAP,
   COLOR,
   M_AMB,
   M_DIF,
   M_SPEC,
   M_SHINE,
   BONES,
   HAS_BONES,
   SHADOW_MAP,
   SHADOW_MAP_TEXTURE,
   REFLECTION_TEXTURE,
   LIGHTNING,
   HAS_SHADOWS,
   HAS_TEXTURE,
   SCREEN_WIDTH,
   SCREEN_HEIGHT,
   USE_BLINN_PHONG,
   OUTPUT_SHADER_TYPE,
   VARY_MATERIAL,
   IS_WATER,

   SUN_DIR,
   SUN_INTENSITY,
   IS_GOD_RAY,
   IS_FIREFLY,
   GOD_RAY_CENTER,
   GOD_RAY_RADIUS,
   FINAL_PASS_POSITION_TEXTURE,
   FINAL_PASS_DIFFUSE_TEXTURE,
   FINAL_PASS_NORMAL_TEXTURE,
   DEF_MODEL_VIEW, /*for deferred*/

   LAST_UNIFORM,
};

inline std::string uniform_name(Uniform uniform) {
   switch (uniform) {
      case Uniform::VIEW:
         return "uViewMatrix";
      case Uniform::MODEL_VIEW:
         return "uModelViewMatrix";
      case Uniform::MODEL_VIEW_PROJECTION:
         return "uModelViewProjectionMatrix";
      case Uniform::MODEL:
         return "uModelMatrix";
      case Uniform::PROJECTION:
         return "uProjectionMatrix";
      case Uniform::NORMAL:
         return "uNormalMatrix";
      case Uniform::TEXTURE:
         return "uTexture";
      case Uniform::HEIGHT_MAP:
         return "uHeightMap";
      case Uniform::HAS_HEIGHT_MAP:
         return "uHasHeightMap";
      case Uniform::HEIGHT_MAP_SCALE:
         return "uHeightMapScale";
      case Uniform::COLOR:
         return "uColor";
      case Uniform::SUN_DIR:
         return "uSunDir";
      case Uniform::SUN_INTENSITY:
         return "uSunIntensity";
      case Uniform::M_AMB:
         return "uMat.ambient";
      case Uniform::M_DIF:
         return "uMat.diffuse";
      case Uniform::M_SPEC:
         return "uMat.specular";
      case Uniform::M_SHINE:
         return "uMat.shine";
      case Uniform::BONES:
         return "uBones";
      case Uniform::HAS_BONES:
         return "uHasBones";
      case Uniform::SHADOW_MAP:
         return "uShadowMap";
      case Uniform::SHADOW_MAP_TEXTURE:
         return "uShadowMapTexture";
      case Uniform::REFLECTION_TEXTURE:
         return "uReflectionTexture";
      case Uniform::LIGHTNING:
         return "uLightning";
      case Uniform::HAS_SHADOWS:
         return "uHasShadows";
      case Uniform::HAS_TEXTURE:
         return "uHasTexture";
      case Uniform::SCREEN_WIDTH:
         return "uScreenWidth";
      case Uniform::SCREEN_HEIGHT:
         return "uScreenHeight";
      case Uniform::USE_BLINN_PHONG:
         return "uUseBlinnPhong";
      case Uniform::IS_GOD_RAY:
         return "uIsGodRay";
      case Uniform::IS_FIREFLY:
         return "uIsFirefly";
      case Uniform::GOD_RAY_CENTER:
         return "uGodRayCenter";
      case Uniform::GOD_RAY_RADIUS:
         return "uGodRayRadius";
      case Uniform::OUTPUT_SHADER_TYPE:
         return "uOutputShaderType";
      case Uniform::VARY_MATERIAL:
         return "uVaryMaterial";
      case Uniform::IS_WATER:
         return "uIsWater";

      case Uniform::FINAL_PASS_POSITION_TEXTURE:
         return "uPositionTexture";
      case Uniform::FINAL_PASS_DIFFUSE_TEXTURE:
         return "uDiffuseTexture";
      case Uniform::FINAL_PASS_NORMAL_TEXTURE:
         return "uNormalTexture";
      case Uniform::DEF_MODEL_VIEW:
         return "uDeferredModelViewMatrix";

      case Uniform::LAST_UNIFORM:
         throw "Unknown uniform type, shouldn't even compile";
   }
}

#endif // UNIFORMS_H_
