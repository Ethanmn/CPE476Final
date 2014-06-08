#include <GL/glew.h>
#include "texture.h"
#include <stdio.h>
#include <string>
#include <memory>

#include <Magick++.h>

using namespace std;

GLTextureID load(const std::string& path);

struct Image {
   unsigned long sizeX;
   unsigned long sizeY;
   char *data;
};

std::string texture_path(TextureType texture) {
   switch (texture) {
      case TextureType::GRASS:
         return "../textures/grass.bmp";
      case TextureType::DEER:
         return "../textures/deer.bmp";
      case TextureType::DAISY:
         return "../textures/daisy.tga";
      case TextureType::ROSE:
         return "../textures/rose.tga";
      case TextureType::BUTTERFLY_RED:
         return "../textures/butterfly_red.bmp";
      case TextureType::BUTTERFLY_PINK:
         return "../textures/butterfly_pink.bmp";
      case TextureType::BUTTERFLY_BLUE:
         return "../textures/butterfly_blue.bmp";
      case TextureType::HEIGHT_MAP:
         return "../textures/height_map.bmp";
      case TextureType::MOON_STONE:
         return "../textures/stone_moon.bmp";
      case TextureType::SUN_STONE:
         return "../textures/stone_sun.bmp";
      case TextureType::TREE:
         return "../textures/tree1.tga";
      case TextureType::RAIN:
         return "../textures/rain.bmp";
      case TextureType::LIGHTNING:
         return "../textures/storm.tga";
      case TextureType::GEM:
         return "../textures/gem.tga";
      case TextureType::SKYBOX_DAY:
         return "../textures/sky_day.tga";
      case TextureType::SKYBOX_NIGHT:
         return "../textures/sky_night.tga";
      case TextureType::LEAF:
         return "../textures/leaf.tga";
      case TextureType::PINECONE:
         return "../textures/pinecone.tga";
      case TextureType::ROCK:
         return "../textures/rock.tga";
      case TextureType::FIREFLY:
         return "../textures/firefly.tga";
      case TextureType::LAST_TEXTURE_TYPE:
         return "";

   }
}

TextureCache::TextureCache() {
   for (size_t i = 0; i < static_cast<size_t>(TextureType::LAST_TEXTURE_TYPE); ++i) {
      const auto texture_type = static_cast<TextureType>(i);
      textures_[texture_type] = load(texture_path(texture_type));
   }
}

GLTextureID TextureCache::getTexture(TextureType texture_type) const {
   return textures_.at(texture_type);
}

GLTextureID TextureCache::load(const std::string& path) const {
   std::unique_ptr<Magick::Image> image;
   Magick::Blob blob;
   try {
      image.reset(new Magick::Image(path.c_str()));
      image->flip();
      image->write(&blob, "RGBA");
   }
   catch (Magick::Error& Error) {
      std::cout << "Error loading texture '" << path << "': " << Error.what() << std::endl;
      exit(EXIT_FAILURE);
   }
   GLuint texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);   //Requires GL 1.4. Removed from GL 3.1 and above.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0,
         GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
   return static_cast<GLTextureID>(texture_id);
}

Texture::Texture(TextureType texture_type, TextureSlot slot) :
   texture_(texture_type),
   texture_slot_(slot)
{
}

Texture::Texture(GLTextureID texture_id, TextureSlot texture_slot) :
   texture_(texture_id),
   texture_slot_(texture_slot)
{
}

void Texture::enable(const TextureCache& texture_cache) const {
   glActiveTexture(GL_TEXTURE0 + texture_slot_);
   if (texture_.type() == typeid(GLTextureID)) {
      glBindTexture(GL_TEXTURE_2D, boost::get<GLTextureID>(texture_));
   } else {
      glBindTexture(GL_TEXTURE_2D, texture_cache.getTexture(boost::get<TextureType>(texture_)));
   }
}
