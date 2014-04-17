#ifndef SHADER_H_
#define SHADER_H_

#include <boost/optional.hpp>
#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>

#include "graphics/gl_adapters/buffer_object.h"
#include "graphics/gl_adapters/gl_shader.h"

#include "graphics/attributes.h"
#include "graphics/uniforms.h"

struct Mesh;
struct ModelViewUniformMatrix;

struct Shader {
   Shader(
         const std::string& name, // e.g. name="ground" for ground.vert and ground.frag
         const std::vector<Attribute>& attributes,
         const std::vector<Uniform>& uniforms);

   void use();

   bool has_attribute(const Attribute& attribute) const {
      return attribute_locations_.count(attribute) > 0;
   }

   bool has_uniform(const Uniform& uniform) const {
      return uniform_locations_.count(uniform) > 0;
   }

   // Helper method. Gets the shader handle and the attribute location of the
   // given attribute, if it can be found.
   boost::optional<std::pair<GLShaderHandle, GLAttributeLocation>> attributeLocation(
         const Attribute& attribute);
   // Helper method. Gets the shader handle and the uniform location of the
   // given uniform.
   boost::optional<std::pair<GLShaderHandle, GLUniformLocation>> uniformLocation(
         const Uniform& uniform);

   template <typename T>
   void sendUniform(const GLUniformLocationMap& uniform, const T& data) {
      gl_shader_.sendUniform(uniform, data);
   }

  private:
   void bindIndexBuffer(const IndexBufferObject& index_buffer);
   void bindAndEnableAttributes(const std::vector<ArrayBufferObject>& array_buffer_objects);
   void disableAttributes(const std::vector<ArrayBufferObject>& array_buffer_objects);

   GLShader gl_shader_;
   std::map<Attribute, GLAttributeLocation> attribute_locations_;
   std::map<Uniform, GLUniformLocation> uniform_locations_;
   const std::string program_name_;
};

#endif // SHADER_H_
