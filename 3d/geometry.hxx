
#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <functional>
#include "buffer_object.hxx"

namespace renderer {
  class geometry {
  protected:
    GLuint theProgram;

    GLuint offsetUniform;
    GLfloat *oVector_uniform;
    GLuint perspectiveMatrixUnif;
    GLfloat *pMatrix_uniform;

  public:
    virtual void draw() = 0;
    virtual void update_global_uniforms() {
      if (theProgram) {
	glUseProgram(theProgram);
	glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, pMatrix_uniform);
	glUniform3fv(offsetUniform, 1, oVector_uniform);
	glUseProgram(0);
      }
    }

    geometry(GLfloat *pMatrix, GLfloat *oVector) :
      pMatrix_uniform(pMatrix),
      oVector_uniform(oVector),
      theProgram(0)
    { }

  };
}

#endif
