#include "glxwindower.hxx"
#include "cstring"
#include "cstdio"
#include "cstdlib"
#include <X11/extensions/XInput2.h>

namespace renderer {

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

  int ctxErrorHandler( Display *dpy, XErrorEvent *ev ) {
    ctxErrorOccurred = true;
    return 0;
  }

  // Helper to check for extension string presence.  Adapted from:
  // http://www.opengl.org/resources/features/OGLextensions/
  bool glxwindower::isExtensionSupported(const char *extList,
					 const char *extension) {
 
    const char *start;
    const char *where, *terminator;
 
    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if ( where || *extension == '\0' )
      return false;
 
    /* It takes a bit of care to be fool-proof about parsing the
       OpenGL extensions string. Don't be fooled by sub-strings,
       etc. */
    for ( start = extList; ; ) {
      where = strstr( start, extension );
 
      if ( !where )
	break;
 
      terminator = where + strlen( extension );
 
      if ( where == start || *(where - 1) == ' ' )
	if ( *terminator == ' ' || *terminator == '\0' )
	  return true;
 
      start = terminator;
    }
 
    return false;
  }

  int glxwindower::create_window(int w, int h) {
    display = XOpenDisplay(NULL);

    window.x = w; window.y = h;
 
    if ( !display ) {
      printf( "Failed to open X display\n" );
      exit(1);
    }
 
    // Check for Xinput
    /* XInput Extension available? */
    int event, error;
    if (!XQueryExtension(display, "XInputExtension", &Xi_opcode,
			 &event, &error)) {
      printf("X Input extension not available.\n");
      exit(1);
    }

    /* Which version of XI2? We support 2.0 */
    int major = 2, minor = 0;
    if (XIQueryVersion(display, &major, &minor) == BadRequest) {
      printf("XI2 not available. Server supports %d.%d\n", major, minor);
      exit(1);
    }

    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if ( !glXQueryVersion( display, &glx_major, &glx_minor ) || 
	 ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
      {
	printf( "Invalid GLX version" );
	exit(1);
      }
 
    // Get a matching FB config
    static int visual_attribs[] =
      {
	GLX_X_RENDERABLE    , True,
	GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
	GLX_RENDER_TYPE     , GLX_RGBA_BIT,
	GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
	GLX_RED_SIZE        , 8,
	GLX_GREEN_SIZE      , 8,
	GLX_BLUE_SIZE       , 8,
	GLX_ALPHA_SIZE      , 8,
	GLX_DEPTH_SIZE      , 24,
	GLX_STENCIL_SIZE    , 8,
	GLX_DOUBLEBUFFER    , True,
	//GLX_SAMPLE_BUFFERS  , 1,
	//GLX_SAMPLES         , 4,
	None
      };
 
    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig( display, DefaultScreen( display ), 
					  visual_attribs, &fbcount );
    if ( !fbc )
      {
	printf( "Failed to retrieve a framebuffer config\n" );
	exit(1);
      }
    // printf( "Found %d matching FB configs.\n", fbcount );
 
    // Pick the FB config/visual with the most samples per pixel
    // Getting XVisualInfos
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
 
    int i;
    for ( i = 0; i < fbcount; i++ )
      {
	XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );
	if ( vi )
	  {
	    int samp_buf, samples;
	    glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS,
				  &samp_buf );
	    glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       ,
				  &samples  );
	    
	    //	    printf( "Matching fbconfig %d, visual ID 0x%2lx:"
	    //	    "SAMPLE_BUFFERS = %d, SAMPLES = %d\n", 
	    //	    i, vi -> visualid, samp_buf, samples );
 
	    if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
	      best_fbc = i, best_num_samp = samples;
	    if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
	      worst_fbc = i, worst_num_samp = samples;
	  }
	XFree( vi );
      }
 
    GLXFBConfig bestFbc = fbc[ best_fbc ];
 
    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbc );
 
    // Get a visual
    XVisualInfo *vi = glXGetVisualFromFBConfig( display, bestFbc );
    //    printf( "Chosen visual ID = 0x%lx\n", vi->visualid );
 
    //    printf( "Creating colormap\n" );
    XSetWindowAttributes swa;
    swa.colormap = cmap = XCreateColormap( display,
					   RootWindow( display, vi->screen ), 
					   vi->visual, AllocNone );
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;
    // | KeyPressMask | ButtonPressMask;
 
    //    printf( "Creating window\n" );
    win = XCreateWindow( display, RootWindow( display, vi->screen ), 
			 100, 100, w, h, 0, vi->depth, InputOutput, 
			 vi->visual, 
			 CWBorderPixel|CWColormap|CWEventMask, &swa );
    if ( !win )
      {
	printf( "Failed to create window.\n" );
	exit(1);
      }

    // Done with the visual info data
    XFree( vi );
 
    XStoreName( display, win, "Accia, simulation" );

    //    printf( "Mapping window\n" );
    XMapWindow( display, win );
 
    // Set Xinput events
    XIEventMask evmasks[1];
    unsigned char mask1[(XI_LASTEVENT + 7)/8];

    memset(mask1, 0, sizeof(mask1));

    /* select for button and key events from all master devices */
    XISetMask(mask1, XI_ButtonPress);
    XISetMask(mask1, XI_ButtonRelease);
    XISetMask(mask1, XI_KeyPress);
    XISetMask(mask1, XI_KeyRelease);

    evmasks[0].deviceid = XIAllMasterDevices;
    evmasks[0].mask_len = sizeof(mask1);
    evmasks[0].mask = mask1;

    XISelectEvents(display, win, evmasks, 1);
    
    int flags = XkbKeySymsMask
      | XkbKeyTypesMask
      | XkbModifierMapMask
      | XkbVirtualModsMask;

    xkb_desc = XkbGetMap (display, flags, XkbUseCoreKbd);

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString( display,
						    DefaultScreen( display ) );
 
    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
      glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");
 
    ctx = 0;
 
    // Install an X error handler so the application won't exit if GL 2.1
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in
    // all threads
    // of a process use the same error handler, so be sure to guard
    // against other
    // threads issuing X commands while this code is running.
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) =
      XSetErrorHandler(&ctxErrorHandler);
 
    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) ||
	 !glXCreateContextAttribsARB )
      {
	printf( "glXCreateContextAttribsARB() not found"
		" ... using old-style GLX context\n" );
	ctx = glXCreateNewContext( display, bestFbc, GLX_RGBA_TYPE, 0, True );
      }
 
    // If it does, try to get a GL 2.1 context!
    else
      {
	int context_attribs[] =
	  {
	    GLX_CONTEXT_MAJOR_VERSION_ARB, 2,
	    GLX_CONTEXT_MINOR_VERSION_ARB, 1,
	    //GLX_CONTEXT_FLAGS_ARB        , 
	    // GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	    None
	  };
 
	//	printf( "Creating context\n" );
	ctx = glXCreateContextAttribsARB( display, bestFbc, 0,
					  True, context_attribs );
 
	// Sync to ensure any errors generated are processed.
	XSync( display, False );
	if ( ctxErrorOccurred || !ctx ) {
	  printf( "Failed to create GL 2.1 context"
		  " ... dieing\n" );
	  exit(-1);
	}
      }
 
    // Sync to ensure any errors generated are processed.
    XSync( display, False );
 
    // Restore the original error handler
    XSetErrorHandler( oldHandler );
 
    if ( ctxErrorOccurred || !ctx )
      {
	printf( "Failed to create an OpenGL context\n" );
	exit(1);
      }
 
    // Verifying that context is a direct context
    if ( ! glXIsDirect ( display, ctx ) )
      {
	printf( "Indirect GLX rendering context obtained\n" );
      }
 
    //    printf( "Making context current\n" );
    glXMakeCurrent( display, win, ctx );

    is_created = true;
  }

  void glxwindower::destroy() {
    if (is_created)
      {
        glXMakeCurrent( display, 0, 0 );
        glXDestroyContext( display, ctx );

        XDestroyWindow( display, win );
        XFreeColormap( display, cmap );
        XCloseDisplay( display );

        is_created = false;
      }
  }

  void glxwindower::swap() {
    glXSwapBuffers ( display, win );
  }

  void glxwindower::process_events() {
    while (XPending(display) > 0) {
      XEvent event;
      XNextEvent(display, &event);
      switch (event.type) {
      case ConfigureNotify:
	window.x = event.xconfigure.width;
	window.y = event.xconfigure.width;
	if (Reshaper)
	  Reshaper(this, event.xconfigure.width, event.xconfigure.height);
	break;
      case GenericEvent: {
	XGenericEventCookie *cookie = &event.xcookie;
	if (cookie -> type == GenericEvent
	    && cookie -> extension == Xi_opcode
	    && XGetEventData(display, cookie)) {
	  XIDeviceEvent *xi_ev = ((XIDeviceEvent *)cookie -> data);
	  switch (cookie -> evtype) {
	  case XI_ButtonPress: {
	    int button = xi_ev -> detail;
	    mouse.x = xi_ev -> event_x;
	    mouse.y = xi_ev -> event_y;
	    if (Mouser)
	      Mouser(this, button, 1, mouse.x, mouse.y);
	    break;
	  }
	  case XI_ButtonRelease: {
	    int button = xi_ev -> detail;
	    mouse.x = xi_ev -> event_x;
	    mouse.y = xi_ev -> event_y;
	    if (Mouser)
	      Mouser(this, button, 0, mouse.x, mouse.y);
	    break;
	  }
	  case XI_KeyPress: {
	    unsigned int mods;
	    KeySym key;
	    XkbTranslateKeyCode(xkb_desc,
				xi_ev -> detail,
				0,
				&mods,
				&key);
	    if (Keyboarder)
	      Keyboarder(this, 1, key);
            break;
	  }
	  case XI_KeyRelease: {
	    unsigned int mods;
	    KeySym key;
	    XkbTranslateKeyCode(xkb_desc,
				xi_ev -> detail,
				0,
				&mods,
				&key);
	    if (Keyboarder)
	      Keyboarder(this, 0, key);
            break;
	  }
	  default:
	    printf("Event type %d %d %d %d received\n", 
		   event.type,
		   cookie -> type,
		   cookie -> evtype,
		   xi_ev -> detail);
	    break;
	  }
	  XFreeEventData(display, cookie);
	}
      }
      default:
	break;
      }
    }
  }

}
