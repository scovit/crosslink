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

  void polymer::update_laplacian(int *laplacian, int *laplacian_index) {
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
    FeedVertexBuffer();
    FeedVertexArrayObjects();
  }

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
  
    update_laplacian(laplacian, laplacian_index);
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
    glUseProgram(theProgram);

    colorUniform = glGetUniformLocation(theProgram, "Color");

    offsetUniform = glGetUniformLocation(theProgram, "offset");

    perspectiveMatrixUnif = glGetUniformLocation(theProgram,
						 "perspectiveMatrix");

    glUniform4f(colorUniform, color[0], color[1], color[2], color[3]);
    glUseProgram(0);

    update_global_uniforms();
  }

  void polymer::FeedVertexBuffer()
  {

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
		 3 * L * sizeof(float)/*sizeof(buffer)*/,
		 buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		 sizeof(GL_SHORT) * nsegdraw, indexData, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void polymer::FeedVertexArrayObjects()
  {

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
