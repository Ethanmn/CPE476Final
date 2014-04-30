#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <iostream>
#include <string>
#include "gl_adapters/gl_types.h"

enum class Textures {
   WATER,
   GRASS,
   DEER,
   BOX
};

inline std::string texture_path(Textures texture) {
   switch (texture) {
      case Textures::WATER:
         return "../textures/water.bmp";
      case Textures::GRASS:
         return "../textures/grass.bmp";
      case Textures::DEER:
         return "../textures/deer1.bmp";
      case Textures::BOX:
         return "../textures/box_texture.bmp";
   }
}

struct Texture {
   GLTextureID texture_id;
   Texture(const std::string& path);
   void enable();
   void disable();
   GLTextureID textureID() const;
};

#endif
