#include <GL/glew.h>
#include "texture.h"
#include <stdio.h>
#include <string>

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
         return "../textures/deer1.bmp";
      case TextureType::FLOWER1:
         return "../textures/flower1.bmp";
      case TextureType::FLOWER2:
         return "../textures/flower2.bmp";
      case TextureType::BUTTERFLY1:
         return "../textures/butterfly1.bmp";
      case TextureType::BUTTERFLY2:
         return "../textures/butterfly2.bmp";
      case TextureType::BUTTERFLY3:
         return "../textures/butterfly3.bmp";
      case TextureType::HEIGHT_MAP:
         return "../textures/height_map.bmp";
      case TextureType::MOON_STONE:
         return "../textures/stone_moon.bmp";
      case TextureType::SUN_STONE:
         return "../textures/stone_sun.bmp";
      case TextureType::TREE:
         return "../textures/tree.bmp";
      case TextureType::RAIN:
         return "../textures/rain.bmp";

      case TextureType::LAST_TEXTURE_TYPE:
         return "";

   }
}

namespace {
   unsigned int getint(FILE *fp) {
      int c, c1, c2, c3;

      /*  get 4 bytes */
      c = getc(fp);
      c1 = getc(fp);
      c2 = getc(fp);
      c3 = getc(fp);

      return ((unsigned int) c) +
         (((unsigned int) c1) << 8) +
         (((unsigned int) c2) << 16) +
         (((unsigned int) c3) << 24);
   }

   unsigned int getshort(FILE *fp){
      int c, c1;

      /* get 2 bytes*/
      c = getc(fp);
      c1 = getc(fp);

      return ((unsigned int) c) + (((unsigned int) c1) << 8);
   }
}

int imageLoad(const std::string& path, Image &image) {
   FILE *file;
   unsigned long size;           /*  size of the image in bytes. */
   unsigned long i;              /*  standard counter. */
   unsigned short int planes;    /*  number of planes in image (must be 1)  */
   unsigned short int bpp;       /*  number of bits per pixel (must be 24) */
   char temp;                    /*  used to convert bgr to rgb color. */

   /*  make sure the file is there. */
   if ((file = fopen(path.c_str(), "rb")) == NULL) {
      printf("File Not Found : %s\n", path.c_str());
      return 0;
   }

   /*  seek through the bmp header, up to the width height: */
   fseek(file, 18, SEEK_CUR);

   /*  read the width */
   image.sizeX = getint (file);

   /*  read the height */
   image.sizeY = getint (file);

   /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
   size = image.sizeX * image.sizeY * 3;

   /*  read the planes */
   planes = getshort(file);
   if (planes != 1) {
      printf("Planes from %s is not 1: %u\n", path.c_str(), planes);
      return 0;
   }

   /*  read the bpp */
   bpp = getshort(file);
   if (bpp != 24) {
      printf("Bpp from %s is not 24: %u\n", path.c_str(), bpp);
      return 0;
   }

   /*  seek past the rest of the bitmap header. */
   fseek(file, 24, SEEK_CUR);

   /*  read the data.  */
   image.data = (char *) malloc(size);
   if (image.data == NULL) {
      printf("Error allocating memory for color-corrected image data");
      return 0;
   }

   if ((i = fread(image.data, size, 1, file)) != 1) {
      printf("Error reading image data from %s.\n", path.c_str());
      return 0;
   }

   for (i = 0; i < size;i += 3) {
      /*  reverse all of the colors. (bgr -> rgb) */
      temp = image.data[i];
      image.data[i] = image.data[i+2];
      image.data[i+2] = temp;
   }

   fclose(file);
   /* Close the file and release the filedes */

   return 1;
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
   Image image;
   cout << "Loading Image: " << path << endl;
   if (!imageLoad(path, image)) {
      exit(1);
   }

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   GLuint texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, image.sizeX, image.sizeY, 0,
         GL_RGB, GL_UNSIGNED_BYTE, image.data);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   free(image.data);
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
