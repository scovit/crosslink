#if NUM_THREADS > 1
 #include <pthread.h>
 #ifdef __APPLE__
 #include "../mac_pthread.h"
 #endif
#endif
#include <cstdio>
#include <cstring>
#include <ctime>
#include <functional>
#include <xmmintrin.h>
#include <immintrin.h>
#define GL_GLEXT_PROTOTYPES
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "../simulazione.h"
#include "gl-subs.hxx"
#include "windower.hxx"
#include "glxwindower.hxx"
#include "buffer_object.hxx"
#include "polymer.hxx"
#include "sphere.hxx"
#include <vector>

float BackGround[4] = {1.0, 1.0, 1.0, 1.0};

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace renderer {

  std::vector<buffered_geom *> objects;
  std::vector<Ibuffer_object *> buffers;

  const GLfloat fzNear = 1.0f;
  const GLfloat fzFar = 8.0f;
  const GLfloat fFrustumScale = 1.5f;
  GLfloat pointsizes = 10.f;

  GLfloat perspectiveMatrix[16] = {
    fFrustumScale, 0,             0,                                  0,
    0,             fFrustumScale, 0,                                  0,
    0,             0,             -2.0f/(fzFar - fzNear),             0,
    0,             0,             -(fzFar + fzNear)/(fzFar - fzNear), 3.0f
  };

  GLfloat offsetVectors[4][3] = {
    { -1.0f,          1.0f,          -4.5f },
    {  1.0f,          1.0f,          -4.5f },
    { -1.0f,         -1.0f,          -4.5f },
    {  1.0f,         -1.0f,          -4.5f },
  };

  static inline void transpose_data(float *bufdest) {
    for (int i = 0; i < N; i += 8) {
      __m256 x = _mm256_load_ps(dots.x + i);
      __m256 y = _mm256_load_ps(dots.y + i);
      __m256 z = _mm256_load_ps(dots.z + i);

      float  *p = bufdest + 3*i; // output pointer
      __m128 *m = (__m128*) p;

      __m256 rxy = _mm256_shuffle_ps(x,y, _MM_SHUFFLE(2,0,2,0)); 
      __m256 ryz = _mm256_shuffle_ps(y,z, _MM_SHUFFLE(3,1,3,1)); 
      __m256 rzx = _mm256_shuffle_ps(z,x, _MM_SHUFFLE(3,1,2,0)); 

      __m256 r03 = _mm256_shuffle_ps(rxy, rzx, _MM_SHUFFLE(2,0,2,0));  
      __m256 r14 = _mm256_shuffle_ps(ryz, rxy, _MM_SHUFFLE(3,1,2,0)); 
      __m256 r25 = _mm256_shuffle_ps(rzx, ryz, _MM_SHUFFLE(3,1,3,1));  
      
      m[0] = _mm256_castps256_ps128( r03 );
      m[1] = _mm256_castps256_ps128( r14 );
      m[2] = _mm256_castps256_ps128( r25 );
      m[3] = _mm256_extractf128_ps( r03 ,1);
      m[4] = _mm256_extractf128_ps( r14 ,1);
      m[5] = _mm256_extractf128_ps( r25 ,1);
    }
  }

  void update_laplacian(buffer_object<GLshort> *dest, 
			int *laplacian, int *laplacian_index) {
    dest -> size = 0;
    for (int i = 0; i < N; i++) {
      for (int q = laplacian_index[i]; q < laplacian_index[i+1]; q++) {
	int c = laplacian[q];
	if(c > i) {    // avoid to compare link two times
	  break;
	}

	if (i != c) {
	  dest -> buffer[dest -> size * 2] = c;
	  dest -> buffer[dest -> size * 2 + 1] = i;
	  dest -> size += 1;
	}
      }
    }
    dest -> size *= 2;
  }

  void reshape (windower *w, GLsizei x, GLsizei y) {
    perspectiveMatrix[0] = fFrustumScale * (y / (float)x);
    perspectiveMatrix[5] = fFrustumScale;

    for(auto i : objects)
      i -> update_global_uniforms();

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
	pointsizes *= zoomspeed;
	break;
      case 5:
	perspectiveMatrix[15] *= zoomspeed;
	pointsizes /= zoomspeed;
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
	for (int i = 0; i < sizeof(offsetVectors) / sizeof(offsetVectors[0]); i++){
	  offsetVectors[i][0] += 5.0 * deltax/wx;
	  offsetVectors[i][1] -= 5.0 * deltay/wy;
	}
	break;
      }
      default:
	break;
      }

    }

    for (auto j : objects)
      j -> update_global_uniforms();

  }

  void display() {
    static int firsttime = 1;

    // update the data for the drawings
    pthread_spin_lock (&spinsum);

    for (auto i : buffers)
      i -> upload();

    if (firsttime) {
      firsttime = 0;
      pthread_barrier_wait(&firstbarr);
    };
    
    pthread_spin_unlock (&spinsum);

    // clear the buffer
    glClearColor(BackGround[0], BackGround[1], BackGround[2], BackGround[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // draw the polymer
    for(auto i : objects)
      i -> draw();

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

  //
  // The graphic buffers
  //

  // The polymer, need to be update every time
  renderer::buffer_object<GLfloat> *polydata;
  polydata = new renderer::buffer_object<GLfloat>(3 * N,
						  GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
  polydata -> prepare_data = [polydata]() -> void {
    renderer::transpose_data(polydata -> buffer);
  }; polydata -> upload();
  renderer::buffers.push_back(polydata);

  // The laplacian
  renderer::buffer_object<GLshort> *indexdata;
  indexdata = new renderer::buffer_object<GLshort>(2 * ODGRMAX * N,
						   GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
  renderer::update_laplacian(indexdata, lpl, lpl_index);
  indexdata -> upload();

  // The index which just draws everything in term of points
  renderer::buffer_object<GLshort> *alldata;
  alldata = new renderer::buffer_object<GLshort>(N,
						 GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
  for (int i = 0; i < N; i++)
    alldata -> buffer[i] = i;
  alldata -> upload();

  //
  // The drawing programs
  //

  renderer::polymer *Poly;
  Poly = new renderer::polymer(renderer::perspectiveMatrix,
			       renderer::offsetVectors[0], 
			       polydata, indexdata,
			       0.0f, 0.0f, 1.0f, 1.0f);
  renderer::objects.push_back(Poly);

  Poly = new renderer::polymer(renderer::perspectiveMatrix,
  			       renderer::offsetVectors[1], 
  			       polydata, indexdata,
  			       0.0f, 1.0f, 0.0f, 1.0f);
  renderer::objects.push_back(Poly);

  renderer::sphere *Sphall;
  Sphall = new renderer::sphere(renderer::perspectiveMatrix,
				renderer::offsetVectors[2],
				// CACCIUTO
				&renderer::pointsizes,
				// 24,
				polydata, alldata,
				1.0f, 0.0f, 0.0f, 1.0f);
  renderer::objects.push_back(Sphall);

#ifdef XLINK
  // The crosslink laplacian
  renderer::buffer_object<GLshort> *crxdata;
  crxdata = new renderer::buffer_object<GLshort>(2 * ODGRMAX * N,
						 GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
  crxdata -> prepare_data = [crxdata]() -> void {
    renderer::update_laplacian(crxdata, crx, crx_index);
  }; crxdata -> upload();
  renderer::buffers.push_back(crxdata);

  renderer::polymer *Xlink = new renderer::polymer(renderer::perspectiveMatrix,
  						   renderer::offsetVectors[0], 
  						   polydata, crxdata,
  						   1.0f, 0.0f, 0.0f, 1.0f);
  renderer::objects.push_back(Xlink);
#endif

#ifdef LOCALIZED
  renderer::buffer_object<GLshort> *Interacting;
  renderer::buffer_object<GLshort> *NonInteracting;
  Interacting    = new renderer::buffer_object<GLshort>(locnum,
							GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
  NonInteracting = new renderer::buffer_object<GLshort>(N - locnum,
							GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

  Interacting -> size = 0;
  NonInteracting -> size = 0;
  for (int i = 0; i < N; i ++)
    if (is_loc_interacting(i)) {
      Interacting -> buffer[Interacting -> size] = i;
      Interacting -> size++;
    } else {
      NonInteracting -> buffer[NonInteracting -> size] = i;
      NonInteracting -> size++;
    }
  Interacting -> upload(); NonInteracting -> upload();

  renderer::sphere *Sph;
  Sph = new renderer::sphere(renderer::perspectiveMatrix,
			     renderer::offsetVectors[0],
			     // CACCIUTO
			     &renderer::pointsizes,
			     // 24,
			     polydata, Interacting,
			     1.0f, 0.0f, 0.0f, 1.0f);
  renderer::sphere *SphUni;
  SphUni = new renderer::sphere(renderer::perspectiveMatrix,
				renderer::offsetVectors[0],
				// CACCIUTO
				&renderer::pointsizes,
				// 24,
				polydata, NonInteracting,
				0.0f, 1.0f, 1.0f, 0.3f);
  renderer::objects.push_back(Sph);
  renderer::objects.push_back(SphUni);
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
