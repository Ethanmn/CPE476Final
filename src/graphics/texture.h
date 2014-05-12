#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <iostream>
#include <string>
#include "gl_adapters/gl_types.h"

enum class Textures {
   WATER,
   GRASS,
   DEER,

   HEIGHT_MAP,
};

inline std::string texture_path(Textures texture) {
   switch (texture) {
      case Textures::WATER:
         return "../textures/water.bmp";
      case Textures::GRASS:
         return "../textures/grass.bmp";
      case Textures::DEER:
         return "../textures/deer1.bmp";
      case Textures::HEIGHT_MAP:
         return "../textures/height_map.bmp";
   }
}

struct Texture {
   Texture(const std::string& path);
   void enable() const;
   void disable() const;
   GLTextureID textureID() const;

   private:
      GLTextureID texture_id;

};


#endif
