#ifndef DRAW_TEMPLATE_H_
#define DRAW_TEMPLATE_H_

#include <glm/glm.hpp>
#include "texture.h"
#include "shaders.h"
#include "mesh.h"

struct Material;
struct Bone; 
   
struct DrawTemplate {
   ShaderType shader_type;
   Mesh mesh;
   Texture texture;
   /* Material and Bones are currently in Mesh */
   //Material material;
   //Bone bones;   
};

struct Drawable {
   DrawTemplate draw_template;
   std::vector<glm::mat4> model_transforms;
};

#endif
