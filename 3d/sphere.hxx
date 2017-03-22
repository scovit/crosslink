
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "buffered_geom.hxx"

namespace renderer {

  struct sphere_params {
    GLfloat color[4];
    GLfloat pointsize;

    sphere_params(GLfloat r, GLfloat g, GLfloat b, GLfloat a,
		  GLfloat pointsize) :
      color { r, g, b, a },
      pointsize(pointsize)
    { };
  };


  class sphere : public buffered_geom {
  private:
    GLuint colorUniform;
    GLuint lightdirUniform;
    GLuint pointsizeUniform;

    void InitializeProgram();
  public:

    sphere_params *params;

    void draw();

    void update_global_uniforms();

    sphere(GLfloat *pMatrix, GLfloat *oVector,
	   buffer_object<GLfloat> *buff, buffer_object<GLshort> *ind,
	   sphere_params *params) : 
      buffered_geom(pMatrix, oVector, buff, ind),
      params(params)
    {
      InitializeProgram();
    };

  };
}

#endif
