
#ifndef BUFFER_OBJ_HPP
#define BUFFER_OBJ_HPP

#include <functional>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>

namespace renderer {

  class Ibuffer_object {
  public:
    virtual void upload() = 0;
    virtual void glBind() = 0;
  };

  template<typename T> class buffer_object : public Ibuffer_object {
  private:

    GLuint BufferObject;

  public:

    GLsizei size;
    T *restrict buffer;
    std::function<void()> prepare_data;

    GLenum target;
    GLenum usage;

    void upload() {
      if (prepare_data)
	prepare_data();

      glBindBuffer(target, BufferObject);
      glBufferData(target, size * sizeof(T),
		   buffer, usage);
      glBindBuffer(target, 0);

    }

    void glBind() {
      glBindBuffer(target, BufferObject);
    }

    buffer_object(GLsizei size, GLenum target, GLenum usage) :
      size(size),
      prepare_data(nullptr),
      target(target),
      usage(usage)
    {
      int err = (posix_memalign((void **)&buffer, 32, size * sizeof(T)));
      if(err) {
	fprintf(stderr, "Error allocating memory\n");
	exit(-1);
      }

      glGenBuffers(1, &BufferObject);
    }

    buffer_object(buffer_object<T> *shared, GLenum target, GLenum usage) :
      size(shared -> size),
      prepare_data(nullptr),
      target(target),
      usage(usage),
      buffer(shared -> buffer)
    {
      glGenBuffers(1, &BufferObject);
    }

    buffer_object(T *buffer, GLsizei size, GLenum target, GLenum usage) :
      size(size),
      prepare_data(nullptr),
      target(target),
      usage(usage),
      buffer(buffer)
    {
      glGenBuffers(1, &BufferObject);
    }

  };
}
#endif
