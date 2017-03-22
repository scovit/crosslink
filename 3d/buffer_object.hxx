
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

    buffer_object(GLsizei n, GLenum targe, GLenum usag) :
      size(n),
      prepare_data(nullptr),
      target(targe),
      usage(usag)
    {
      int err = (posix_memalign((void **)&buffer, 32, size * sizeof(T)));
      if(err) {
	fprintf(stderr, "Error allocating memory\n");
	exit(-1);
      }

      glGenBuffers(1, &BufferObject);
    }
  };
}
#endif
