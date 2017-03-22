
#ifndef BUFFERED_GEOM_HPP
#define BUFFERED_GEOM_HPP

#include <functional>
#include "buffer_object.hxx"
#include "geometry.hxx"

namespace renderer {
  class buffered_geom : public geometry {
  protected:

    GLuint vaoObject;
    buffer_object<GLfloat> *buffer;
    buffer_object<GLshort> *index;

  public:
    virtual void draw() = 0;
    virtual void update_global_uniforms() {
      geometry::update_global_uniforms();
    };

    buffered_geom(GLfloat *pMatrix, GLfloat *oVector,
		  buffer_object<GLfloat> *buffer, buffer_object<GLshort> *index) :
      geometry(pMatrix, oVector),
      buffer(buffer),
      index(index)
    {
      glGenVertexArrays(1, &vaoObject);
      glBindVertexArray(vaoObject);

      buffer -> glBind();

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

      index -> glBind();

      glBindVertexArray(0);
    }

  };
}

#endif
