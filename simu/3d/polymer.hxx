
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

    void InitializeData(int *laplacian,
			int *laplacian_index, int max_out);
    void InitializeVertexBuffer();
    void InitializeVertexArrayObjects();
    void InitializeProgram();
  public:

    void draw();

    polymer(GLfloat *pMatrix, GLfloat *oVector,
	    int n, int *laplacian, int *laplacian_index, int max_out) : 
      buffered_geom(pMatrix, oVector, n) {
      InitializeData(laplacian, laplacian_index, max_out);
      InitializeVertexBuffer();
      InitializeVertexArrayObjects();
      InitializeProgram();
    };

    ~polymer();
  };
}

#endif
