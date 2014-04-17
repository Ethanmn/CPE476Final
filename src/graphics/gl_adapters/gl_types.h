#ifndef GL_TYPES_H_
#define GL_TYPES_H_

#include <boost/serialization/strong_typedef.hpp>
#include <GL/glew.h>
#include <map>

BOOST_STRONG_TYPEDEF(GLint, GLAttributeLocation)
BOOST_STRONG_TYPEDEF(GLint, GLUniformLocation)
BOOST_STRONG_TYPEDEF(GLuint, GLShaderHandle)
BOOST_STRONG_TYPEDEF(GLuint, GLBufferHandle)

typedef std::map<GLShaderHandle, GLAttributeLocation> GLAttributeLocationMap;
typedef std::map<GLShaderHandle, GLUniformLocation> GLUniformLocationMap;

#endif // GL_TYPES_H_
