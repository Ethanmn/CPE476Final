#include "graphics/gl_adapters/uniform_matrix.h"

#include "graphics/gl_adapters/gl_shader.h"

void UniformMatrix::sendToShader(GLShader& shader) const {
   shader.uniform(uniforms_, matrix);
}
