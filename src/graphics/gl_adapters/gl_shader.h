#ifndef GL_SHADER_H_
#define GL_SHADER_H_

#include <glm/glm.hpp>
#include <string>

#include "graphics/gl_adapters/gl_types.h"

struct ArrayBufferObject;
struct IndexBufferObject;

struct GLShader {
   static GLShader compileAndLinkShader(
         const std::string& vertex_shader_path,
         const std::string& fragment_shader_path);
   GLShader(GLShaderHandle linked_program) : program_(linked_program) {}

   void use();

   template <typename T>
   void uniform(const GLUniformLocationMap& uniforms, const T& data);

   void bindAndEnableAttribute(const ArrayBufferObject& array_buffer);
   void disableAttribute(const ArrayBufferObject& array_buffer);

   GLAttributeLocation getAttributeLocation(const std::string& attribute);
   GLUniformLocation getUniformLocation(const std::string& uniform);

   GLShaderHandle program() { return program_; }

  private:
   GLShaderHandle program_;
};

#endif // GL_SHADER_H_
