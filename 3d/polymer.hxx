
#ifndef POLYMER_HPP
#define POLYMER_HPP

#include "buffered_geom.hxx"
#include "buffer_object.hxx"

namespace renderer {
  class polymer : public buffered_geom {
  private:
    GLuint colorUniform;

    GLfloat color[4];

    void InitializeProgram();
  public:

    void draw();

    polymer(GLfloat *pMatrix, GLfloat *oVector,
	    buffer_object *buff, buffer_object *ind,
	    GLfloat r, GLfloat g, GLfloat b, GLfloat a) : 
      buffered_geom(pMatrix, oVector, buff, ind) {
      color[0] = r; color[1] = g; color[2] = b; color[3] = a;

      InitializeProgram();
    };

  };
}

#endif
