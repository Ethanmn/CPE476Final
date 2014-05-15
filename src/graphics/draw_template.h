#ifndef DRAW_TEMPLATE_H_
#define DRAW_TEMPLATE_H_

#include <glm/glm.hpp>
#include <texture.h>
#include <mesh.h>

struct Material;
struct Bone; 
   
struct DrawTemplate {
   Mesh mesh;
   Texture texture;
   /* Material and Bones are currently in Mesh */
   //Material material;
   //Bone bones;   
};

#endif
