#define GL_GLEXT_PROTOTYPES
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "windower.hxx"


#ifndef GLXWINDOWER_HPP
#define GLXWINDOWER_HPP

namespace renderer {

  static bool ctxErrorOccurred = false;

  static int ctxErrorHandler( Display *dpy, XErrorEvent *ev );

  class glxwindower : public windower {
  private:
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*,
						       GLXFBConfig, 
						       GLXContext,
						       Bool,
						       const int*);

    Display *display;
    Window win;
    GLXContext ctx;
    Colormap cmap;
    int Xi_opcode;
    XkbDescPtr xkb_desc;

    bool isExtensionSupported(const char *extList, const char *extension);
  public:
    int create_window(int w, int h);
    void swap();
    void destroy();
    void process_events();
  };

}
#endif
