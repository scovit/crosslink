#include <cstdlib>
#include <cstdio>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <glm/glm.hpp>
#include "gl-subs.hxx"
#include "sphere.hxx"

namespace renderer {

  void sphere::InitializeProgram() {
    std::vector<std::string> inputList = { "position" };

    std::vector<GLuint> shaderList = {
        renderer::LoadShader(GL_VERTEX_SHADER,
	                     "data/sphere.vert"),
        renderer::LoadShader(GL_FRAGMENT_SHADER,
			     "data/sphere.frag")
    };

    theProgram = renderer::CreateProgram(shaderList, inputList);
    glUseProgram(theProgram);

    offsetUniform = glGetUniformLocation(theProgram, "offset");

    perspectiveMatrixUnif = glGetUniformLocation(theProgram,
						 "perspectiveMatrix");

    colorUniform = glGetUniformLocation(theProgram, "Color");
    lightdirUniform = glGetUniformLocation(theProgram, "lightDir");
    pointsizeUniform = glGetUniformLocation(theProgram, "pointsize");

    glUniform4f(colorUniform, color[0], color[1], color[2], color[3]);
    glUniform3f(lightdirUniform, 0.0f, 0.0f, 1.0f);
    glUniform1f(pointsizeUniform, pointsize);
    glUseProgram(0);

    update_global_uniforms();
  }

  void sphere::draw() {
    bool psize = glIsEnabled(GL_VERTEX_PROGRAM_POINT_SIZE);
    if (!psize)
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glUseProgram(theProgram);
    glBindVertexArray(vaoObject);

    glDrawElements(GL_POINTS, index -> size / sizeof(GLshort),
		   GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    if (!psize)
      glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);

  }
  
}
