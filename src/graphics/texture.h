#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <iostream>
#include <string>
#include "gl_adapters/gl_types.h"

enum class Textures {
   WATER,
   GRASS,
   DEER,
   BUTTERFLY,
   HEIGHT_MAP,
   MOON_STONE,
   SUN_STONE,
   SKYBOX
};

inline std::string texture_path(Textures texture) {
   switch (texture) {
      case Textures::WATER:
         return "../textures/water.bmp";
      case Textures::GRASS:
         return "../textures/grass.bmp";
      case Textures::DEER:
         return "../textures/deer1.bmp";
      case Textures::BUTTERFLY:
         return "../textures/butterfly.bmp";
      case Textures::HEIGHT_MAP:
         return "../textures/height_map.bmp";
      case Textures::MOON_STONE:
         return "../textures/stone_moon.bmp";
      case Textures::SUN_STONE:
         return "../textures/stone_sun.bmp";
      case Textures::SKYBOX:
         return "../textures/skybox.bmp";

   }
}

struct Texture {
   Texture(const std::string& path);
   Texture(GLTextureID texture_id);
   void enable() const;
   GLTextureID textureID() const;

  private:
   GLTextureID texture_id;
   GLTextureID texture_slot;
};


#endif
