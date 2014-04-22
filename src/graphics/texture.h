#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <iostream>
#include <string>

enum class Textures {
   WATER,
   GRASS
};

inline std::string texture_path(Textures texture) {
   switch (texture) {
      case Textures::WATER:
         return "../textures/water.bmp";
      case Textures::GRASS:
         return "../textures/grass.bmp";
   }
}

struct Texture {
   int texture_id;
   Texture(const std::string& path);
   void enable();
   void disable();
};

#endif
