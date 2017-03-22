
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "segments.hxx"
#include "buffer_object.hxx"
#include "geometry.hxx"

namespace renderer {

  struct matrix_params {
    GLfloat color[4];
    GLfloat pointsize;
    int dim;

    matrix_params(GLfloat r, GLfloat g, GLfloat b, GLfloat a,
		  GLfloat pointsize, int dim) :
      color { r, g, b, a },
      pointsize(pointsize),
      dim(dim)
    { };
  };

  class matrix : public geometry {
  private:

    GLuint vaoObject;
    buffer_object<GLshort> *ind;

    GLuint wdnUniform;
    static const GLfloat wdn;
    GLuint colorUniform;
    GLuint dimUniform;
    GLuint pointsizeUniform;

    static GLfloat borderverticesbuf[];
    static GLshort borderindexesbuf[];

    static buffer_object<GLfloat> *bordervertices;
    static buffer_object<GLshort> *borderindex;

    segments *border;

    void InitializeProgram();
    void InitializeBorder();
    void InitializeVao();
  public:

    matrix_params *params;

    void draw();

    void update_global_uniforms();

    matrix(GLfloat *pMatrix, GLfloat *oVector,
	   buffer_object<GLshort> *ind,
	   matrix_params *params) : 
      geometry(pMatrix, oVector),
      params(params),
      ind(ind)
    {
      InitializeBorder();
      InitializeVao();
      InitializeProgram();
    };

  };
}

#endif
