#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <iostream>
#include <string>

enum class Texture {
   WATER
};

inline std::string texture_path(Texture texture) {
   switch (texture) {
      case Texture::WATER:
         return "../textures/water.bmp";
   }
}

typedef struct Image {
   unsigned long sizeX;
   unsigned long sizeY;
   char *data;
} Image;

void initTexture();
void enableTexture(int texture_id);
void disableTexture();
int loadTexture(const std::string& path);
static unsigned int getint(FILE *fp);
static unsigned int getshort(FILE *fp);
int imageLoad(const std::string& path, Image &image);

#endif
