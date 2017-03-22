#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include "gl-subs.hxx"
#include "matrix.hxx"
#include "segments.hxx"

namespace renderer {

  const GLfloat matrix::wdn = .6f;

  GLfloat matrix::borderverticesbuf[3 * 4] = {
    -wdn/2 * 1.02, -wdn/2 * 1.02, 0.0f,  // A
    wdn/2 * 1.02, -wdn/2 * 1.02, 0.0f,   // B
    wdn/2 * 1.02,  wdn/2 * 1.02, 0.0f,   // C
    -wdn/2 * 1.02,  wdn/2 * 1.02, 0.0f   // D
  };

  GLshort matrix::borderindexesbuf[2 * 5] = {
    0, 1, 1, 2, 2, 3, 3, 0, 1, 3
  };

  buffer_object<GLfloat> *matrix::bordervertices = nullptr;
  buffer_object<GLshort> *matrix::borderindex    = nullptr;

  void matrix::update_global_uniforms() {
    geometry::update_global_uniforms();
    border -> update_global_uniforms();

    if (params) {
      glUseProgram(theProgram);
      glUniform4f(colorUniform, params -> color[0], params -> color[1],
    		  params -> color[2], params -> color[3]);
      glUniform1f(dimUniform, float(params -> dim));
      glUniform1f(pointsizeUniform, params -> pointsize);
      glUniform1f(wdnUniform, wdn);
      glUseProgram(0);
    }
  }


  void matrix::InitializeBorder() {
    if (!bordervertices) {
      bordervertices = new buffer_object<GLfloat>(borderverticesbuf, sizeof(borderverticesbuf) / sizeof(borderverticesbuf[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
      borderindex    = new buffer_object<GLshort>(borderindexesbuf, sizeof(borderindexesbuf) / sizeof(borderindexesbuf[0]), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

      bordervertices -> upload();
      borderindex -> upload();
    }

    border = new segments(pMatrix_uniform, oVector_uniform,
			  bordervertices, borderindex,
			  new renderer::segments_params(0.f, 0.f, 0.f, 1.f,
							1.5f, 1, 0xFFFF));
  }

  void matrix::InitializeVao() {
    glGenVertexArrays(1, &vaoObject);
    glBindVertexArray(vaoObject);

    ind -> glBind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
  }

  void matrix::InitializeProgram() {

    std::vector<std::string> inputList = { "position" };

    std::vector<GLuint> shaderList = {
            renderer::LoadShader(GL_VERTEX_SHADER,
				 "data/Matrix.vert"),
            renderer::LoadShader(GL_GEOMETRY_SHADER,
				 "data/Matrix.geom"),
    	    renderer::LoadShader(GL_FRAGMENT_SHADER,
				 "data/Standard.frag")
    };

    theProgram = renderer::CreateProgram(shaderList, inputList);
    glUseProgram(theProgram);

    colorUniform = glGetUniformLocation(theProgram, "Color");
    offsetUniform = glGetUniformLocation(theProgram, "offset");
    perspectiveMatrixUnif = glGetUniformLocation(theProgram,
						 "perspectiveMatrix");
    wdnUniform = glGetUniformLocation(theProgram, "wdn");
    dimUniform = glGetUniformLocation(theProgram, "dim");
    pointsizeUniform = glGetUniformLocation(theProgram, "pointsize");
    glUseProgram(0);

    update_global_uniforms();
  }

  void matrix::draw() {
    border -> draw();

    bool psize = glIsEnabled(GL_VERTEX_PROGRAM_POINT_SIZE);
    if (!psize)
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glUseProgram(theProgram);
    glBindVertexArray(vaoObject);
 
    glDrawArrays(GL_POINTS, 0, ind -> size);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    if (!psize)
      glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
  }

}
