
#ifndef SEGMENTS_HPP
#define SEGMENTS_HPP

#include "buffered_geom.hxx"
#include "buffer_object.hxx"

namespace renderer {

  struct segments_params {
    GLfloat color[4];
    GLfloat width;
    GLint stipple_factor;
    GLushort stipple_pattern;

    segments_params(GLfloat r, GLfloat g, GLfloat b, GLfloat a,
		   GLfloat width,
		   GLint stipple_factor,
		   GLushort stipple_pattern) :
      color { r, g, b, a },
      width(width),
      stipple_factor(stipple_factor),
      stipple_pattern(stipple_pattern)
    { };
  };

  class segments : public buffered_geom {
  private:
    GLuint colorUniform;

    void InitializeProgram();
  public:

    segments_params *params;

    void draw();

    void update_global_uniforms();

    segments(GLfloat *pMatrix, GLfloat *oVector,
	    buffer_object<GLfloat> *buffer, buffer_object<GLshort> *index,
	    segments_params *params) : 
      buffered_geom(pMatrix, oVector, buffer, index),
      params(params)
    {
      InitializeProgram();
    };

  };
}

#endif
