#ifndef SHADER_H_
#define SHADER_H_

#include <boost/optional.hpp>
#include <GL/glew.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "graphics/gl_adapters/buffer_object.h"
#include "graphics/gl_adapters/gl_shader.h"

#include "graphics/attributes.h"
#include "graphics/uniforms.h"
#include "graphics/location_maps.h"

struct AffineUniforms;
struct Mesh;
struct ModelViewUniformMatrix;

struct Shader {
   Shader(
         const std::string& name, // e.g. name="ground" for ground.vert and ground.frag
         const std::vector<Attribute>& attributes,
         const std::vector<Uniform>& uniforms);

   void use();

   void drawMesh(const Mesh& mesh);

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
   void sendUniform(const Uniform& uniform, const UniformLocationMap& uniforms, const T& data) {
      if (!uniforms.count(uniform)) {
         noUniformError(uniform);
         return;
      }
      try {
         gl_shader_.sendUniform(uniforms.at(uniform), data);
      } catch (std::exception&) {
         noUniformError(uniform);
         return;
      }
   }

  private:
   void bindIndexBuffer(const IndexBufferObject& index_buffer);
   void bindAndEnableAttributes(const std::vector<ArrayBufferObject>& array_buffer_objects);
   void disableAttributes(const std::vector<ArrayBufferObject>& array_buffer_objects);

   void noUniformError(const Uniform& uniform) const {
      if (uniform_errors_.count(uniform))
         return;
      uniform_errors_.insert(uniform);
      std::cerr << std::endl << "Could not find " << uniform_name(uniform);
      std::cerr << " in program " << program_name_ << std::endl;
      std::cerr << "Make sure it is in the program's list of uniforms in ";
      std::cerr << "Shaders.cpp" << std::endl << std::endl;
      return;
   }

   GLShader gl_shader_;
   std::map<Attribute, GLAttributeLocation> attribute_locations_;
   std::map<Uniform, GLUniformLocation> uniform_locations_;
   mutable std::set<Uniform> uniform_errors_;
   const std::string program_name_;
};

#endif // SHADER_H_
