#ifndef UNIFORMS_H_
#define UNIFORMS_H_

#include <string>

enum class Uniform {
   FIRST_UNIFORM,
   MODEL_VIEW = FIRST_UNIFORM,
   VIEW,
   PROJECTION,
   NORMAL,
   TEXTURE,
   COLOR,
   SUN_DIR,
   SUN_INTENSITY,
   M_AMB,
   M_DIF,
   M_SPEC,
   M_SHINE,
   LAST_UNIFORM,
};

inline std::string uniform_name(Uniform uniform) {
   switch (uniform) {
      case Uniform::VIEW:
         return "uViewMatrix";
      case Uniform::MODEL_VIEW:
         return "uModelViewMatrix";
      case Uniform::PROJECTION:
         return "uProjectionMatrix";
      case Uniform::NORMAL:
         return "uNormalMatrix";
      case Uniform::TEXTURE:
         return "uTexture";
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
      case Uniform::LAST_UNIFORM:
         throw "Unknown uniform type, shouldn't even compile";
   }
}

#endif // UNIFORMS_H_
