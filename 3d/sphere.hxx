
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "buffered_geom.hxx"

namespace renderer {
  class sphere : public buffered_geom {
  private:
    GLuint colorUniform;
    GLuint lightdirUniform;
    GLuint pointsizeUniform;

    GLfloat pointsize;
    GLfloat *ptrpointsize;
    GLfloat color[4];

    void InitializeProgram();
  public:

    void draw();

    void update_global_uniforms() {
      buffered_geom::update_global_uniforms();

      pointsize = *ptrpointsize;
      glUseProgram(theProgram);
      glUniform1f(pointsizeUniform, pointsize);
      glUseProgram(0);
    }

    sphere(GLfloat *pMatrix, GLfloat *oVector, GLfloat *psize,
	   buffer_object<GLfloat> *buff, buffer_object<GLshort> *ind,
	   GLfloat r, GLfloat g, GLfloat b, GLfloat a) : 
      buffered_geom(pMatrix, oVector, buff, ind) {
      ptrpointsize = psize;
      pointsize = *ptrpointsize;
      color[0] = r; color[1] = g; color[2] = b; color[3] = a;

      InitializeProgram();
    };

  };
}

#endif
