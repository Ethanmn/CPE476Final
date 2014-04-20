#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif

#include "texture.h"
#include <stdio.h>
#include <string>
using namespace std;

#define TEXTURE_ID_OFFSET 33984

namespace {
   int texture_id = 0;
}

void initTexture() {
   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void enableTexture(int texture_id) {
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(TEXTURE_ID_OFFSET + texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
}

void disableTexture() {
   glDisable(GL_TEXTURE_2D);
}

int loadTexture(const std::string& path) {
   Image image;
   cout << "Loading Image: " << path << endl;
   if (!imageLoad(path, image)) {
      exit(1);
   }
   glBindTexture(GL_TEXTURE_2D, texture_id);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, image.sizeX, image.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   free(image.data);
   return texture_id++;
}

static unsigned int getint(FILE *fp) {
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

static unsigned int getshort(FILE *fp){
   int c, c1;
   
   /* get 2 bytes*/
   c = getc(fp);
   c1 = getc(fp);
   
   return ((unsigned int) c) + (((unsigned int) c1) << 8);
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
