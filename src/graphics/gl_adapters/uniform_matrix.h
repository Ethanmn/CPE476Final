#ifndef UNIFORM_MATRIX_H_
#define UNIFORM_MATRIX_H_

#include <glm/glm.hpp>

#include "graphics/gl_adapters/gl_types.h"

struct GLShader;

struct UniformMatrix {
   UniformMatrix(
         const glm::mat4& matrix,
         const GLUniformLocationMap& uniforms) :
      matrix(matrix),
      uniforms_(uniforms) {}

   void sendToShader(GLShader& shader) const;
   glm::mat4 matrix;

  private:
   GLUniformLocationMap uniforms_;
};

struct AffineUniforms {
   UniformMatrix model_view_uniform, normal_uniform;
};

#endif // UNIFORM_MATRIX_H_
