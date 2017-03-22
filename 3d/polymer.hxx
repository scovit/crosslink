
#ifndef POLYMER_HPP
#define POLYMER_HPP

#include "buffered_geom.hxx"
#include "buffer_object.hxx"

namespace renderer {

  struct polymer_params {
    GLfloat color[4];
    GLfloat width;
    GLint stipple_factor;
    GLushort stipple_pattern;

    polymer_params(GLfloat r, GLfloat g, GLfloat b, GLfloat a,
		   GLfloat width,
		   GLint stipple_factor,
		   GLushort stipple_pattern) :
      color { r, g, b, a },
      width(width),
      stipple_factor(stipple_factor),
      stipple_pattern(stipple_pattern)
    { };
  };

  class polymer : public buffered_geom {
  private:
    GLuint colorUniform;

    void InitializeProgram();
  public:

    polymer_params *params;

    void draw();

    void update_global_uniforms();

    polymer(GLfloat *pMatrix, GLfloat *oVector,
	    buffer_object<GLfloat> *buff, buffer_object<GLshort> *ind,
	    polymer_params *params) : 
      buffered_geom(pMatrix, oVector, buff, ind),
      params(params)
    {
      InitializeProgram();
    };

  };
}

#endif
