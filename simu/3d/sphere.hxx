
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

    void InitializeData();
    void InitializeProgram();
  public:
    // Those 3 parent class
    void draw();

    void setPointsize(float Pointsize);
    float getPointsize() {
      return pointsize;
    };

    sphere(GLfloat *pMatrix, GLfloat *oVector, int n, int psize) : 
      buffered_geom(pMatrix, oVector, n) {
      pointsize = psize;
      InitializeData();
      InitializeProgram();
    };

    ~sphere();
  };
}

#endif
