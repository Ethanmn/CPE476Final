#ifndef UNIFORMS_H_
#define UNIFORMS_H_

#include <string>

enum class Uniform {
   MODEL,
   VIEW,
   MODEL_VIEW,
   PROJECTION,
   NORMAL,
   TEXTURE,
   COLOR,
   SUNDIR,
   SUNINTENSITY,
   MATERIAL
};

inline std::string uniform_name(Uniform uniform) {
   switch (uniform) {
      case Uniform::MODEL:
         return "uModelMatrix";
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
      case Uniform::SUNDIR:
         return "uSunDir";
      case Uniform::SUNINTENSITY:
         return "uSunIntensity";
      case Uniform::MATERIAL:
         return "uMat";
   }
}

#endif // UNIFORMS_H_
