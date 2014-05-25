#include "draw_template.h"

void switchTextureAndBlinn(Drawable *drawObj) {
   if(drawObj->draw_template.shader_type == ShaderType::TEXTURE)
      drawObj->draw_template.shader_type = ShaderType::TEXTURE_BLINN;
   //else if(drawObj->draw_template.shader_type == ShaderType::TEXTURE_BLINN)
      //drawObj->draw_template.shader_type = ShaderType::TEXTURE;
}
