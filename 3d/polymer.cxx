#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include "gl-subs.hxx"
#include "polymer.hxx"

namespace renderer {

  void polymer::InitializeData(int *laplacian,
			       int *laplacian_index, int max_out) {

    int err = (posix_memalign((void **)&buffer, 16,
			      sizeof(GLfloat) * 3 * L));
    err += posix_memalign((void **)&indexData, 16,
			   sizeof(GLshort) * max_out * L);

    if(err) {
      fprintf(stderr, "Error allocating memory\n");
      exit(-1);
    }
  
    nsegdraw = 0;
    for (int i = 0; i < L; i++) {
      for (int q = laplacian_index[i]; q < laplacian_index[i+1]; q++) {
	int c = laplacian[q];
	if(c > i) {    // avoid to compare link two times
	  break;
	}

	indexData[nsegdraw] = i;
	indexData[nsegdraw+1] = c;
	nsegdraw += 2;
      }
    }
  }

  polymer::~polymer() {
    free(buffer); free(indexData);
  }

  void polymer::InitializeProgram() {

    std::vector<std::string> inputList = { "position" };

    std::vector<GLuint> shaderList = {
            renderer::LoadShader(GL_VERTEX_SHADER,
				 "data/Standard.vert"),
    	    renderer::LoadShader(GL_FRAGMENT_SHADER,
				 "data/Standard.frag")
    };

    theProgram = renderer::CreateProgram(shaderList, inputList);

    offsetUniform = glGetUniformLocation(theProgram, "offset");

    perspectiveMatrixUnif = glGetUniformLocation(theProgram,
						 "perspectiveMatrix");

    update_global_uniforms();
  }

  void polymer::InitializeVertexBuffer()
  {
    glGenBuffers(1, &vertexBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
		 3 * L * sizeof(float)/*sizeof(buffer)*/,
		 buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 sizeof(GL_SHORT) * nsegdraw, indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void polymer::InitializeVertexArrayObjects()
  {
    glGenVertexArrays(1, &vaoObject);

    glBindVertexArray(vaoObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

    glBindVertexArray(0);
  }

  void polymer::draw() {

    glUseProgram(theProgram);
    glBindVertexArray(vaoObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
		 3 * L * sizeof(float),
		 buffer, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    float linew;
    glGetFloatv(GL_LINE_WIDTH, &linew);
    glLineWidth(2.0f);

    glDrawElements(GL_LINES, nsegdraw, GL_UNSIGNED_SHORT, 0);

    glLineWidth(linew);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

  }
}
