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

  void polymer::update_global_uniforms() {
    buffered_geom::update_global_uniforms();

    if (params) {
      glUseProgram(theProgram);
      glUniform4f(colorUniform, params -> color[0], params -> color[1],
		  params -> color[2], params -> color[3]);
      glUseProgram(0);
    }
  }

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
    glUseProgram(0);

    update_global_uniforms();
  }

  void polymer::draw() {
    bool stipples = glIsEnabled(GL_LINE_STIPPLE);
    if (!stipples)
      glEnable(GL_LINE_STIPPLE);

    glUseProgram(theProgram);
    glBindVertexArray(vaoObject);

    float linew;
    glGetFloatv(GL_LINE_WIDTH, &linew);
    glLineWidth(params -> width);
    glLineStipple(params -> stipple_factor,
		  params -> stipple_pattern);

    glDrawElements(GL_LINES, index -> size,
		   GL_UNSIGNED_SHORT, 0);

    glLineWidth(linew);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    if (!stipples)
      glDisable(GL_LINE_STIPPLE);
  }
}
