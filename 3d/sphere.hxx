
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "buffered_geom.hxx"

namespace renderer {
  class sphere : public buffered_geom {
  private:

    GLuint vertexBufferObject;
    GLuint vaoObject;

    GLuint colorUniform;
    GLuint lightdirUniform;
    GLuint pointsizeUniform;

    GLfloat pointsize;
    GLfloat color[4];

    void InitializeData();
    void InitializeProgram();
  public:
    // Those 3 parent class
    void draw();

    void setPointsize(float Pointsize);
    float getPointsize() {
      return pointsize;
    };

    sphere(GLfloat *pMatrix, GLfloat *oVector, int n, int psize,
	   GLfloat r, GLfloat g, GLfloat b, GLfloat a) : 
      buffered_geom(pMatrix, oVector, n) {
      pointsize = psize;
      color[0] = r; color[1] = g; color[2] = b; color[3] = a;
      InitializeData();
      InitializeProgram();
    };

    ~sphere();
  };
}

#endif
