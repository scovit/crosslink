#include <pthread.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <functional>
#include <xmmintrin.h>
#define GL_GLEXT_PROTOTYPES
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "../simulazione.h"
#include "gl-subs.hxx"
#include "windower.hxx"
#include "glxwindower.hxx"
#include "polymer.hxx"
#include "sphere.hxx"

float BackGround[4] = {1.0, 1.0, 1.0, 1.0};

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace renderer {

  polymer *Poly;
#ifdef LOCALIZED
  sphere  *Sph;
  sphere  *SphUni;
#endif

  const GLfloat fzNear = 1.0f;
  const GLfloat fzFar = 8.0f;
  const GLfloat fFrustumScale = 1.0f;

  GLfloat perspectiveMatrix[16] = {
    fFrustumScale, 0,             0,                                  0,
    0,             fFrustumScale, 0,                                  0,
    0,             0,             -2.0f/(fzFar - fzNear),             0,
    0,             0,             -(fzFar + fzNear)/(fzFar - fzNear), 3.0f
  };

  GLfloat offsetVector[3] = {
    0.0f,          0.0f,          -4.5f
  };

  void transpose_data(GLfloat *bufdest) {
    for (int i = 0; i < N; i += 4) {
      __m128 x = _mm_load_ps(dots.x + i);
      __m128 y = _mm_load_ps(dots.y + i);
      __m128 z = _mm_load_ps(dots.z + i);

      __m128 x0x2y0y2 = _mm_shuffle_ps(x,y, _MM_SHUFFLE(2,0,2,0));
      __m128 y1y3z1z3 = _mm_shuffle_ps(y,z, _MM_SHUFFLE(3,1,3,1));
      __m128 z0z2x1x3 = _mm_shuffle_ps(z,x, _MM_SHUFFLE(3,1,2,0));

      __m128 rx0y0z0x1= _mm_shuffle_ps(x0x2y0y2,z0z2x1x3,
				       _MM_SHUFFLE(2,0,2,0)); 
      __m128 ry1z1x2y2= _mm_shuffle_ps(y1y3z1z3,x0x2y0y2,
				       _MM_SHUFFLE(3,1,2,0)); 
      __m128 rz2x3y3z3= _mm_shuffle_ps(z0z2x1x3,y1y3z1z3,
				       _MM_SHUFFLE(3,1,3,1)); 

      _mm_stream_ps(bufdest + 3*i + 0, rx0y0z0x1 );
      _mm_stream_ps(bufdest + 3*i + 4, ry1z1x2y2 );
      _mm_stream_ps(bufdest + 3*i + 8, rz2x3y3z3 );
    }
  }

#ifdef LOCALIZED
  void interaction_data(GLfloat *bufdestI, GLfloat *bufdestNI) {
    int k1 = 0;
    int k2 = 0;
    for (int i = 0; i < N; i ++)
      if (is_loc_interacting(i)) {
	bufdestI[k1++] = dots.x[i];
	bufdestI[k1++] = dots.y[i];
	bufdestI[k1++] = dots.z[i];
      } else {
	bufdestNI[k2++] = dots.x[i];
	bufdestNI[k2++] = dots.y[i];
	bufdestNI[k2++] = dots.z[i];
      }
  }
#endif

  void reshape (windower *w, GLsizei x, GLsizei y) {
    perspectiveMatrix[0] = fFrustumScale * (y / (float)x);
    perspectiveMatrix[5] = fFrustumScale;

    Poly -> update_global_uniforms();
#ifdef LOCALIZED
    Sph -> update_global_uniforms();
    SphUni -> update_global_uniforms();
#endif

    glViewport(0, 0, (GLsizei) x, (GLsizei) y);

  }

  void keypress(windower *w, int t, int i) {
    if (i == 'q' && t == 0)
      pthread_exit(NULL);
  }

  void mousebutton(windower *w, int i, int t, double x, double y) {
    static double pressedx;
    static double pressedy;

    if (t) /* Pressed */{
      float zoomspeed = 1.5;
      switch (i) {
      case 1:
        pressedx = x;
	pressedy = y;
	break;
      case 4:
	perspectiveMatrix[15] /= zoomspeed;
#ifdef LOCALIZED
	Sph -> setPointsize(Sph -> getPointsize() * zoomspeed);
	SphUni -> setPointsize(SphUni -> getPointsize() * zoomspeed);
#endif
	break;
      case 5:
	perspectiveMatrix[15] *= zoomspeed;
#ifdef LOCALIZED
	Sph -> setPointsize(Sph -> getPointsize() / zoomspeed);
	SphUni -> setPointsize(SphUni -> getPointsize() / zoomspeed);
#endif
	break;
      default:
	break;
      }

    } else /* Released */ {
      switch (i) {
      case 1: {
	double deltax = x - pressedx;
	double deltay = y - pressedy;
	int wx, wy;
	w -> getWindowsize(&wx, &wy);
	offsetVector[0] += 5.0 * deltax/wx;
	offsetVector[1] -= 5.0 * deltay/wy;
	break;
      }
      default:
	break;
      }

    }

    Poly -> update_global_uniforms();
#ifdef LOCALIZED
    Sph -> update_global_uniforms();
    SphUni -> update_global_uniforms();
#endif
  }

  void display() {
    static int firsttime = 1;

    // update the data for the drawings
    pthread_spin_lock (&spinsum);

    transpose_data(Poly -> buffer);
#ifdef LOCALIZED
    interaction_data(Sph -> buffer, SphUni -> buffer);
#endif
    if (firsttime) {
      firsttime = 0;
      pthread_barrier_wait(&firstbarr);
    };
    
    pthread_spin_unlock (&spinsum);

    // clear the buffer
    glClearColor(BackGround[0], BackGround[1], BackGround[2], BackGround[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // draw the polymer

    Poly -> draw();
#ifdef LOCALIZED
    Sph -> draw();
    SphUni -> draw();
#endif

    glFlush();

  }

}


extern "C" void *glumain(void *threadarg) {
  pthread_barrier_wait(&startbarr);
  int argc = ((struct thread_data *) threadarg) -> argc;
  char **argv = ((struct thread_data *) threadarg) -> argv;

  renderer::windower *Window = new renderer::glxwindower();
  
  Window -> create_window(800, 800);
  Window -> Reshaper = renderer::reshape;
  Window -> Keyboarder = renderer::keypress;
  Window -> Mouser = renderer::mousebutton;

  // Implement zoom with the mouse

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_POINT_SPRITE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  renderer::Poly = new renderer::polymer(renderer::perspectiveMatrix,
					 renderer::offsetVector, 
					 N, lpl, lpl_index, ODGRMAX);

#ifdef LOCALIZED
  renderer::Sph = new renderer::sphere(renderer::perspectiveMatrix,
				       renderer::offsetVector, locnum,
				       // CACCIUTO
				       10,
                                       // 24,
				       1.0f, 0.0f, 0.0f, 1.0f);
  renderer::SphUni = new renderer::sphere(renderer::perspectiveMatrix,
					  renderer::offsetVector, N - locnum,
					  // CACCIUTO
					  10,
                                          // 24,
					  0.0f, 1.0f, 1.0f, 0.3f);
#endif

  struct timespec tim, tim2;
  while(true) {

    Window -> process_events();

    renderer::display();

    Window -> swap();
    
    tim.tv_sec = 1L;
    tim.tv_nsec = 100000000L;
    nanosleep(&tim , &tim2);
  }
 
  Window -> destroy();
}
