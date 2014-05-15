#ifndef DRAW_SHADERS_
#define DRAW_SHADERS_

#include <glm/gtc/matrix_transform.hpp>   
#include "draw_template.h"
#include <vector>

struct DrawShader {
   DrawShader() : uniforms(shaders.getUniformLocationMap()), 
      projectionMatrix(glm::perspective(80.0f, 640.0f/480.0f, 0.1f, 500.f)) {}
   void Draw(glm::mat4 viewMatrix, std::vector<Drawable>);
   
   private:
      Shaders shaders;
      UniformLocationMap uniforms;
      const glm::mat4 projectionMatrix;
};


#endif
