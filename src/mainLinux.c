#include <stdio.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <sys/time.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "diokol.h"

// default window width
#if !defined(INITIAL_WINDOW_WIDTH)
#define INITIAL_WINDOW_WIDTH 640
#endif
// default window height
#if !defined(INITIAL_WINDOW_HEIGHT)
#define INITIAL_WINDOW_HEIGHT 480
#endif

Display *dpy;
Window glwin;

//int Visual_attribs[] = {
int attributes[] = {
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    // request a double-buffered color buffer with the maximum number of bits per component
    GLX_DOUBLEBUFFER, True,
    GLX_RED_SIZE, 1,
    GLX_GREEN_SIZE, 1,
    GLX_BLUE_SIZE, 1,
    GLX_ALPHA_SIZE, 1,
    GLX_DEPTH_SIZE, 1,
    GLX_STENCIL_SIZE, 1,
    GLX_SAMPLE_BUFFERS, 1,
    GLX_SAMPLES, 8,
    None
};

//int attributes[100];

//memcpy( attributes, Visual_attribs, sizeof(Visual_attribs));

XVisualInfo             *vi;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;
Atom atom_DELWIN;
Atom atom_PROTOCOLS;

int x11_fd;
fd_set in_fds;

void prepareOpenGL(int width,int height) {
    initOpenVG(width,height);
}

void drawRect() {
  drawScene();
  glFinish();
}

int dragged = 0;

void processEvent(XEvent *ev) {
    
    switch (ev->type) {
            
        case Expose:
          XGetWindowAttributes(dpy, glwin, &gwa);
          glViewport(0, 0, gwa.width, gwa.height);
          drawRect();
          glXSwapBuffers(dpy, glwin);
          break;
            
        case KeyPress:
            Dkl_KeyPressed(XLookupKeysym(&ev->xkey, 0),0);
            break;
            
        case KeyRelease:
            Dkl_KeyReleased(XLookupKeysym(&ev->xkey, 0),0);
            break;
            
        case ButtonPress:
            dragged = 1;
            if (ev->xbutton.button == Button1)
                Dkl_MousePressed(ev->xbutton.x, ev->xbutton.y,0);
            else
                if (ev->xbutton.button == Button2 || ev->xbutton.button == Button3)
                    Dkl_MousePressed(ev->xbutton.x, ev->xbutton.y,1);
            break;
            
        case ButtonRelease:
            dragged = 0;
            if (ev->xbutton.button == Button1)
                Dkl_MouseReleased(ev->xbutton.x, ev->xbutton.y,0);
            else
                if (ev->xbutton.button == Button2 || ev->xbutton.button == Button3)
                    Dkl_MouseReleased(ev->xbutton.x, ev->xbutton.y,1);
            break;
            
        case MotionNotify:
            Dkl_MouseMoved(ev->xmotion.x, ev->xmotion.y,dragged);
            break;
            
        case ResizeRequest:
            vg_resize(ev->xresizerequest.width,ev->xresizerequest.height);
            break;
            
        case ClientMessage:
            if ((((XClientMessageEvent *)ev)->message_type == atom_PROTOCOLS) &&
                (((XClientMessageEvent *)ev)->data.l[0] == (long)atom_DELWIN))
                done = 1;
            break;
    }
}

static int resizeWindow(int w,int h) {
    XResizeWindow(dpy,glwin,w,h);
}

void initOpenVG( int width, int height) {
    vgCreateContextSH(width,height);
    vg_init(width,height);
    lua_init();
}

void endOpenVG() {
    vgDestroyContextSH();
    lua_destroy();
}

void killWindow(void) {
    glXMakeContextCurrent(dpy, 0, 0, 0);
    glXDestroyWindow(dpy, glwin);
    glXDestroyContext(dpy, glc);
    XDestroyWindow(dpy, RootWindow(dpy, vi->screen));
    XCloseDisplay(dpy);
}

int main(int argc, char *argv[]) {
	
 struct timeval tv;

 XSetWindowAttributes winAttr;
 int             dummy;
 GLXFBConfig fbconfig = 0;
 int         fbcount;
 GLXFBConfig *fbc;
 GLXFBConfig *fbConfigs;
 int fbConfigsCount = 0;
int scr;

 dpy = XOpenDisplay(NULL);
 if (dpy == NULL){
    fprintf(stderr, "could not open display\n");
    exit(1);
 }
    
scr = DefaultScreen(dpy);

 if(!glXQueryExtension(dpy, &dummy, &dummy)){
    fprintf(stderr, "could not open display\n");
    exit(1);
 }

 fbConfigs = glXChooseFBConfig(dpy, scr, attributes, &fbConfigsCount);
    if ((!fbConfigs) || (fbConfigsCount < 1)) {
        // try 6 samples
        attributes[21] = 6;
        fbConfigs = glXChooseFBConfig(dpy, scr, attributes, &fbConfigsCount);
        if ((!fbConfigs) || (fbConfigsCount < 1)) {
            // try 4 samples
            attributes[21] = 4;
            fbConfigs = glXChooseFBConfig(dpy, scr, attributes, &fbConfigsCount);
            if ((!fbConfigs) || (fbConfigsCount < 1)) {
                // try 2 samples
                attributes[21] = 2;
                fbConfigs = glXChooseFBConfig(dpy, scr, attributes, &fbConfigsCount);
                if ((!fbConfigs) || (fbConfigsCount < 1)) {
                    // no fsaa
                    attributes[19] = 0;
                    attributes[21] = 0;
                    fbConfigs = glXChooseFBConfig(dpy, scr, attributes, &fbConfigsCount);
                    if ((!fbConfigs) || (fbConfigsCount < 1)) {
                        // try with no stencil buffer
                        attributes[17] = 0;
                        fbConfigs = glXChooseFBConfig(dpy, scr, attributes, &fbConfigsCount);
                        if ((!fbConfigs) || (fbConfigsCount < 1)) {
                            fprintf(stderr, "Cannot find a suitable framebuffer configuration.\n");
                            return 0;
                        }
                    }
                }
            }
        }
    }

 //vi = glXChooseVisual(dpy, DefaultScreen(dpy), attributes);
 vi = glXGetVisualFromFBConfig(dpy, fbConfigs[0]);
 if (vi == NULL) {
    fprintf(stderr, "could not get visual\n");
    exit(1);
 }
 
 glc = glXCreateContext(dpy, vi,  None, GL_TRUE);
 if (glc == NULL) {
    fprintf(stderr, "could not create rendering context\n");
    exit(1);
 }

 winAttr.event_mask = StructureNotifyMask | KeyPressMask ;
 winAttr.background_pixmap = None ;
 winAttr.background_pixel  = 0    ;
 winAttr.border_pixel      = 0    ;
    
 winAttr.colormap = XCreateColormap(dpy, RootWindow(dpy, vi->screen),
                                       vi->visual, AllocNone);
    
 winAttr.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask | ResizeRedirectMask;
 
 glwin = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, 0, vi->depth, InputOutput, vi->visual,
    winAttr.event_mask, &winAttr);
 XSetStandardProperties(dpy, glwin, "Diokol", "Diokol", None, argv,argc, NULL);

 glXMakeCurrent(dpy, glwin, glc);

 XMapWindow(dpy, glwin);
    
 XSelectInput(dpy, glwin,
                 ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
                 ButtonPressMask | ButtonReleaseMask  | StructureNotifyMask  | ResizeRedirectMask
                 );
    
 prepareOpenGL(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
 
 x11_fd = ConnectionNumber(dpy);

 while(!done) {
    FD_ZERO(&in_fds);
    FD_SET(x11_fd, &in_fds);
     
     tv.tv_usec = 1000000/frameRate;
     tv.tv_sec = 0;
    
     int num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);
     if (num_ready_fds >= 0) {
         drawRect();
         glXSwapBuffers(dpy, glwin);
     }
     
     if (XPending(dpy)) {
         XNextEvent(dpy, &xev);
         processEvent(&xev);
      }
   }
  endOpenVG();
  killWindow();
  return EXIT_SUCCESS;
}
