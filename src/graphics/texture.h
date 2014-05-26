#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <iostream>
#include <boost/variant.hpp>
#include <string>
#include "gl_adapters/gl_types.h"

enum class TextureType {
   GRASS,
   DEER,
   FLOWER1,
   FLOWER2,
   BUTTERFLY1,
   BUTTERFLY2,
   BUTTERFLY3,
   HEIGHT_MAP,
   MOON_STONE,
   SUN_STONE,
   TREE,
   RAIN,

   LAST_TEXTURE_TYPE,
};

std::string texture_path(TextureType texture);

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

struct TextureCache {
   TextureCache();
   GLTextureID getTexture(TextureType texture_type) const;

  private:
   GLTextureID load(const std::string& path) const;

   std::map<TextureType, GLTextureID> textures_;
};

struct Texture {
   Texture(TextureType texture_type, TextureSlot slot);
   Texture(GLTextureID texture_id, TextureSlot slot);
   void enable(const TextureCache& texture_cache) const;
   int texture_slot() const { return texture_slot_; }

  private:
   boost::variant<GLTextureID, TextureType> texture_;
   int texture_slot_;
};


#endif
