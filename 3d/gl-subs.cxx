//Copyright (C) 2010-2012 by Jason L. McKesson
//This file is licensed under the MIT License.

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include "gl-subs.hxx"

namespace renderer {

  GLuint CreateShader(GLenum eShaderType, const GLchar *strFileData)
  {
    GLuint shader = glCreateShader(eShaderType);
    glShaderSource(shader, 1, &strFileData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
      {
	GLint infoLogLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

	GLchar *strInfoLog = new GLchar[infoLogLength + 1];
	glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

	const char *strShaderType = NULL;
	switch(eShaderType)
	  {
	  case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
	  case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
	  case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
	  }

	fprintf(stderr, "Compile failure in %s shader:\n%s\n",
		strShaderType, strInfoLog);
	free(strInfoLog);
      }

    return shader;
  }

  GLuint LoadShader(GLenum eShaderType, const char *strFilename)
  {
    FILE *shaderFile = fopen(strFilename, "r");
    if (!shaderFile) {
      fprintf(stderr, "Unable to open file %s", strFilename);
      return 0;
    }

    // Get file length
    fseek(shaderFile, 0, SEEK_END);
    int fileLen = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);

    //Allocate memory
    char *filebuffer=new char[fileLen+1];


    //Read file contents into filebuffer
    if (fread(filebuffer, fileLen, 1, shaderFile) != 1) {
      fprintf(stderr, "Weird read error %s", strFilename);
      return 0;
    }

    filebuffer[fileLen] = '\0';
    fclose(shaderFile);

    int shader = CreateShader(eShaderType, filebuffer);
    free(filebuffer);
    return shader;
  }

  GLuint CreateProgram(std::vector<GLuint> shaderList,
                       std::vector<std::string> &iList)
  {
    GLuint program = glCreateProgram();

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
      glAttachShader(program, shaderList[iLoop]);

    for(size_t iLoop = 0; iLoop < iList.size(); iLoop++){
      glBindAttribLocation(program, iLoop, iList[iLoop].c_str());
    }

    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
      {
	GLint infoLogLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

	GLchar *strInfoLog = new GLchar[infoLogLength + 1];
	glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
	fprintf(stderr, "Linker failure: %s\n", strInfoLog);
	free(strInfoLog);
      }

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
      glDetachShader(program, shaderList[iLoop]);

    return program;
  }

}
