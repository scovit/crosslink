
#ifndef BUFFERED_GEOM_HPP
#define BUFFERED_GEOM_HPP

namespace renderer {
  class buffered_geom {
  protected:
    int L;

    GLuint theProgram;
    GLuint offsetUniform;
    GLfloat *oVector_uniform;
    GLuint perspectiveMatrixUnif;
    GLfloat *pMatrix_uniform;
  public:
    GLfloat *restrict buffer;

    virtual void draw() = 0;
    void update_global_uniforms() {
      if (theProgram) {
	glUseProgram(theProgram);
	glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, pMatrix_uniform);
	glUniform3fv(offsetUniform, 1, oVector_uniform);
	glUseProgram(0);
      }
    };

    buffered_geom(GLfloat *pMatrix, GLfloat *oVector, int n) {
      L = n;
      pMatrix_uniform = pMatrix;
      oVector_uniform = oVector;
      theProgram = 0;
      buffer = nullptr;
    }

  };
}

#endif
