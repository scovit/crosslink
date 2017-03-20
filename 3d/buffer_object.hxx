
#ifndef BUFFER_OBJ_HPP
#define BUFFER_OBJ_HPP

#include <functional>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>

namespace renderer {
  class buffer_object {
  private:

    GLuint BufferObject;

  public:

    GLsizeiptr size;
    void *restrict buffer;
    std::function<void()> prepare_data;

    GLenum target;
    GLenum usage;

    void upload() {
      if (prepare_data)
	prepare_data();

      glBindBuffer(target, BufferObject);
      glBufferData(target, size,
		   buffer, usage);
      glBindBuffer(target, 0);

    }

    void glBind() {
      glBindBuffer(target, BufferObject);
    }

    buffer_object(GLsizeiptr n, GLenum targe, GLenum usag) {
      size = n;
      prepare_data = nullptr;

      int err = (posix_memalign((void **)&buffer, 16, size));
      if(err) {
	fprintf(stderr, "Error allocating memory\n");
	exit(-1);
      }
      target = targe;
      usage  = usag;

      glGenBuffers(1, &BufferObject);
    }
  };
}
#endif
