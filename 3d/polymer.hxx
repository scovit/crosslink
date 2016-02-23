
#ifndef POLYMER_HPP
#define POLYMER_HPP

#include "buffered_geom.hxx"

namespace renderer {
  class polymer : public buffered_geom {
  private:
    GLshort *indexData;
    int nsegdraw;

    GLuint vertexBufferObject;
    GLuint indexBufferObject;
    GLuint vaoObject;

    GLuint colorUniform;

    GLfloat color[4];

    void InitializeData(int *laplacian,
			int *laplacian_index, int max_out);
    void InitializeProgram();
    void FeedVertexBuffer();
    void FeedVertexArrayObjects();
  public:

    void draw();

    void update_laplacian(int *laplacian, int *laplacian_index);

    polymer(GLfloat *pMatrix, GLfloat *oVector,
	    int n, int *laplacian, int *laplacian_index, int max_out,
	    GLfloat r, GLfloat g, GLfloat b, GLfloat a) : 
      buffered_geom(pMatrix, oVector, n) {
      color[0] = r; color[1] = g; color[2] = b; color[3] = a;
      glGenBuffers(1, &vertexBufferObject);
      glGenBuffers(1, &indexBufferObject);
      glGenVertexArrays(1, &vaoObject);
      InitializeData(laplacian, laplacian_index, max_out);
      InitializeProgram();
    };

    ~polymer();
  };
}

#endif
