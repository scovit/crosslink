#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include "gl-subs.hxx"
#include "polymer.hxx"

namespace renderer {

  void polymer::InitializeProgram() {

    std::vector<std::string> inputList = { "position" };

    std::vector<GLuint> shaderList = {
            renderer::LoadShader(GL_VERTEX_SHADER,
				 "data/Standard.vert"),
    	    renderer::LoadShader(GL_FRAGMENT_SHADER,
				 "data/Standard.frag")
    };

    theProgram = renderer::CreateProgram(shaderList, inputList);
    glUseProgram(theProgram);

    colorUniform = glGetUniformLocation(theProgram, "Color");

    offsetUniform = glGetUniformLocation(theProgram, "offset");

    perspectiveMatrixUnif = glGetUniformLocation(theProgram,
						 "perspectiveMatrix");

    glUniform4f(colorUniform, color[0], color[1], color[2], color[3]);
    glUseProgram(0);

    update_global_uniforms();
  }

  void polymer::draw() {

    glUseProgram(theProgram);
    glBindVertexArray(vaoObject);

    float linew;
    glGetFloatv(GL_LINE_WIDTH, &linew);
    glLineWidth(2.0f);

    glDrawElements(GL_LINES, index -> size / sizeof(GLshort),
		   GL_UNSIGNED_SHORT, 0);

    glLineWidth(linew);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

  }
}
