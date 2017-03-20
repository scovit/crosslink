
#ifndef BUFFERED_GEOM_HPP
#define BUFFERED_GEOM_HPP

#include <functional>
#include "buffer_object.hxx"

namespace renderer {
  class buffered_geom {
  protected:
    GLuint theProgram;

    GLuint offsetUniform;
    GLfloat *oVector_uniform;
    GLuint perspectiveMatrixUnif;
    GLfloat *pMatrix_uniform;

    GLuint vaoObject;
    buffer_object<GLfloat> *buffer;
    buffer_object<GLshort> *index;

  public:
    virtual void draw() = 0;
    virtual void update_global_uniforms() {
      if (theProgram) {
	glUseProgram(theProgram);
	glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, pMatrix_uniform);
	glUniform3fv(offsetUniform, 1, oVector_uniform);
	glUseProgram(0);
      }
    };

    buffered_geom(GLfloat *pMatrix, GLfloat *oVector,
		  buffer_object<GLfloat> *bobj, buffer_object<GLshort> *iobj) {
      buffer = bobj;
      index  = iobj;

      pMatrix_uniform = pMatrix;
      oVector_uniform = oVector;
      theProgram = 0;

      glGenVertexArrays(1, &vaoObject);
      glBindVertexArray(vaoObject);

      buffer -> glBind();

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

      index -> glBind();

      glBindVertexArray(0);
    }

  };
}

#endif
