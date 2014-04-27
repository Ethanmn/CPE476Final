#include "gl_shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "gl_types.h"
#include <iostream>
#include <sstream>

#include "graphics/gl_adapters/array_buffer_object.h"

namespace {
   void uniformMatrix4fv(GLint location, const GLfloat *value) {
      glUniformMatrix4fv(location, 1, GL_FALSE, value);
   }

   std::string shader_type_str(GLenum shader_type) {
      switch(shader_type)
      {
         case GL_VERTEX_SHADER:   return "vertex";
         case GL_GEOMETRY_SHADER: return "geometry";
         case GL_FRAGMENT_SHADER: return "fragment";
      }
      return "";
   }

   std::string read_file(const std::string& path) {
      std::ifstream source_stream(path);
      std::stringstream source_buffer;
      source_buffer << source_stream.rdbuf();
      return source_buffer.str();
   }

   GLuint compileShaderFromSource(GLenum shader_type, const std::string& path) {
      std::string file_source(read_file(path));

      GLuint shader = glCreateShader(shader_type);
      const char *source = file_source.c_str();
      glShaderSource(shader, 1, &source, NULL);

      glCompileShader(shader);
      GLint status;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
      if (status == GL_FALSE) {
         GLint log_length;
         glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

         GLchar info_log[4096];
         glGetShaderInfoLog(shader, log_length, NULL, info_log);

         printf("Compile failure in %s shader:\n%s\n", shader_type_str(shader_type).c_str(), info_log);
         exit(EXIT_FAILURE);
      }

      return shader;
   }
}

//static
GLShader GLShader::compileAndLinkShader(
      const std::string& vertex_shader_path,
      const std::string& fragment_shader_path) {
   GLuint vertex_shader = compileShaderFromSource(GL_VERTEX_SHADER, vertex_shader_path);
   GLuint fragment_shader = compileShaderFromSource(GL_FRAGMENT_SHADER, fragment_shader_path);

   GLuint program = glCreateProgram();
   glAttachShader(program, vertex_shader);
   glAttachShader(program, fragment_shader);

   glLinkProgram(program);
   GLint status;
   glGetProgramiv(program, GL_LINK_STATUS, &status);
   if (status == GL_FALSE) {
      GLint log_length;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

      GLchar info_log[4096];
      glGetProgramInfoLog(program, log_length, NULL, info_log);
      printf("Shader linker failure: %s\n", info_log);
   }

   glDetachShader(program, vertex_shader);
   glDetachShader(program, fragment_shader);

   return GLShaderHandle(program);
}

void GLShader::use() {
   glUseProgram(program_);
}

template <>
void GLShader::sendUniform<glm::mat4>(
      const GLUniformLocationMap& uniforms,
      const glm::mat4& data) {
   uniformMatrix4fv(uniforms.at(program_), glm::value_ptr(data));
}

template <>
void GLShader::sendUniform<GLTextureID>(
      const GLUniformLocationMap& uniforms,
      const GLTextureID& data) {
   glUniform1i(uniforms.at(program_), static_cast<GLint> (data));
}

template <>
void GLShader::sendUniform<glm::vec3>(
      const GLUniformLocationMap& uniforms,
      const glm::vec3& data) {
   glUniform3f(uniforms.at(program_), data.x, data.y, data.z);
}

template <>
void GLShader::sendUniform<glm::vec4>(
      const GLUniformLocationMap& uniforms,
      const glm::vec4& data) {
   glUniform4f(uniforms.at(program_), data.r, data.g, data.b, data.a);
}

template <>
void GLShader::sendUniform<float>(const GLUniformLocationMap& uniforms, const float& data) {
   glUniform1f(uniforms.at(program_), data);
}


template <>
void GLShader::sendUniform<int>(const GLUniformLocationMap& uniforms, const int& data) {
   glUniform1i(uniforms.at(program_), data);
}

void GLShader::bindAndEnableAttribute(const ArrayBufferObject& array_buffer) {
   if (array_buffer.attribute_locations.count(program_) == 0) {
      return;
   }
   glBindBuffer(GL_ARRAY_BUFFER, array_buffer.handle);
   glVertexAttribPointer(
         array_buffer.attribute_locations.at(program_),
         array_buffer.num_components,
         GL_FLOAT, GL_FALSE,
         0,
         nullptr);
   glEnableVertexAttribArray(array_buffer.attribute_locations.at(program_));
}

void GLShader::disableAttribute(const ArrayBufferObject& array_buffer) {
   if (array_buffer.attribute_locations.count(program_) == 0) {
      return;
   }
   glDisableVertexAttribArray(array_buffer.attribute_locations.at(program_));
}

GLAttributeLocation GLShader::getAttributeLocation(const std::string& attribute) {
   GLint a = glGetAttribLocation(program_, attribute.c_str());
   if (a < 0) {
      std::cout << "Could not find attribute location for: " << attribute << " in program: " << program_ << std::endl;
      std::cerr << "Make sure you are actually using it in the program, GLSL optimizes out unused variables." << std::endl;
      exit(EXIT_FAILURE);
   }
   return GLAttributeLocation(a);
}

GLUniformLocation GLShader::getUniformLocation(const std::string& uniform) {
   GLint u = glGetUniformLocation(program_, uniform.c_str());
   if (u < 0) {
      std::cerr << "Could not find uniform location for: " << uniform << " in program: " << program_ << std::endl;
      std::cerr << "Make sure you are actually using it in the program, GLSL optimizes out unused variables." << std::endl;
      exit(EXIT_FAILURE);
   }
   return GLUniformLocation(u);
}
