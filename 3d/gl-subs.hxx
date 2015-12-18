//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.

#ifndef INCLUDE_GL_SUBS
#define INCLUDE_GL_SUBS

#include <vector>
#include <string>

namespace renderer {
  GLuint CreateShader(GLenum eShaderType, const char *strShaderFile);
  GLuint LoadShader(GLenum eShaderType, const char *strShaderFilename);

  GLuint CreateProgram(std::vector<GLuint> shaderList,
                       std::vector<std::string> &iList);

}

#endif
