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

// Not enum class because we are using the values.
// Enum class's are "not" int's (even though they are) and need to be cast to
// ints.
// Enums are ints unless assigned other number values,
// and can be used interchangeably with ints. Also you don't scope in enums.
// e.g.
//
// enum class: TextureSlot::HEIGHT_MAP_TEXTURE
// enum:       HEIGHT_MAP_TEXTURE
enum TextureSlot {
   DIFFUSE_TEXTURE,
   HEIGHT_MAP_TEXTURE,
   SHADOW_MAP_TEXTURE,
   REFLECTION_TEXTURE,
};

struct Texture {
   Texture(const std::string& path, TextureSlot slot);
   Texture(GLTextureID texture_id, TextureSlot slot);
   void enable() const;
   int texture_slot() const { return texture_slot_; }

  private:
   GLTextureID texture_id;
   int texture_slot_;
};


#endif
