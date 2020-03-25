#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <termios.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "EGL/egl.h"
#include "bcm_host.h"
#include "oglinit.h"

#include "diokol.h"

#include <linux/input.h>
#include <fcntl.h>
#include <pthread.h>

// default window width
#if !defined(INITIAL_WINDOW_WIDTH)
    #define INITIAL_WINDOW_WIDTH 640
#endif
// default window height
#if !defined(INITIAL_WINDOW_HEIGHT)
    #define INITIAL_WINDOW_HEIGHT 480
#endif

static struct termios oldChars, newChars;

void initTermios(int echo) //struct termios &oldChars, struct termios &newChars)
{
    fcntl(0, F_SETFL, O_NONBLOCK);
    tcgetattr(0, &oldChars); /* grab old terminal i/o settings */
    newChars = oldChars; /* make new settings same as old settings */
    newChars.c_lflag &= ~ICANON; /* disable buffered i/o */
    newChars.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &newChars); /* use these new terminal i/o settings now */
}

void resetTermios(void)//struct termios &oldChars)
{
    tcsetattr(0, TCSANOW, &oldChars);
}

// OpenVG variables
void *vgContext = NULL;
void *vgWindowSurface = NULL;
// example variables
int displayHelp;
int displayAbout;
char msg[2048];
int done;
unsigned int framesCounter;
unsigned int time0, time1;

unsigned int getTimeMS(void) {

    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);
    return (unsigned int)((tp.tv_sec * 1000) + (tp.tv_usec / 1000));
}

void reshapeFunction(int w, int h) {

    if (w <= 0)
        w = 1;
    if (h <= 0)
        h = 1;

    // resize AmanithG surface
 //   vgPrivSurfaceResizeMZT(vgWindowSurface, w, h);
    // inform example
 //   resizeScene(vgPrivGetSurfaceWidthMZT(vgWindowSurface), vgPrivGetSurfaceHeightMZT(vgWindowSurface));
}

void killWindow(void) {
}

static STATE_T _state, *state = &_state;	// global graphics state
static const int MAXFONTPATH = 500;
static int init_x = 0;		// Initial window position and size
static int init_y = 0;
static unsigned int init_w = 0;
static unsigned int init_h = 0;


// Mouse state structure
typedef struct {
	int fd;
	struct input_event ev;
	VGfloat x, y;
	int left, middle, right;
	int max_x, max_y;
} mouse_t;

mouse_t mouse;			// global mouse state
int left_count = 0;
int quitState = 0;
#define    CUR_SIZ  16					   // cursor size, pixels beyond centre dot

//
// Terminal settings
//

// terminal settings structures
struct termios new_term_attr;
struct termios orig_term_attr;

// saveterm saves the current terminal settings
void saveterm() {
    tcgetattr(fileno(stdin), &orig_term_attr);
}

// rawterm sets the terminal to raw mode
void rawterm() {
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);
}

// restore resets the terminal to the previously saved setting
void restoreterm() {
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
}

// evenThread reads from the mouse input file
void *eventThread(void *arg) {

	// Open mouse driver
	if ((mouse.fd = open("/dev/input/event0", O_RDONLY)) < 0) {
		fprintf(stderr, "Error opening Mouse!\n");
		quitState = 1;
		return &quitState;
	}
	mouse.x = mouse.max_x / 2;			   //Reset mouse
	mouse.y = mouse.max_y / 2;

	while (1) {
		read(mouse.fd, &mouse.ev, sizeof(struct input_event));
		// printf("[%4.0f,%4.0f]\r",mouse.x,mouse.y);

		// Check events
		mouse.left = CUR_SIZ * 2;		   // Reset Mouse button states
		mouse.right = CUR_SIZ * 2;

		if (mouse.ev.type == EV_REL) {
			if (mouse.ev.code == REL_X) {
				mouse.x += (VGfloat) mouse.ev.value;
				if (mouse.x < 0) {
					mouse.x = 0;
				}
				if (mouse.x > mouse.max_x) {
					mouse.x = mouse.max_x;
				}
			}
			if (mouse.ev.code == REL_Y) {	   //This ones goes backwards hence the minus
				mouse.y -= (VGfloat) mouse.ev.value;
				if (mouse.y < 0) {
					mouse.y = 0;
				}
				if (mouse.y > mouse.max_y) {
					mouse.y = mouse.max_y;
				}
			}
		}

		if (mouse.ev.type == EV_KEY) {
			//printf("Time Stamp:%d - type %d, code %d, value %d\n",
			//      mouse.ev.time.tv_usec,mouse.ev.type,mouse.ev.code,mouse.ev.value);
			if (mouse.ev.code == BTN_LEFT) {
				mouse.left = 1;
				//   printf("Left button\n");
				left_count++;
				// printf("User Quit\n");
				// quitState = 1;
				// return &quitState;  //Left mouse to quit
			}
			if (mouse.ev.code == BTN_RIGHT) {
				mouse.right = 1;
				//  printf("Right button\n");
			}
		}
	}
}

static int cur_sx, cur_sy, cur_w, cur_h;	// cursor location and dimensions
static int cur_saved = 0;	// amount of data saved in cursor image backup

// saveCursor saves the pixels under the mouse cursor
void saveCursor(VGImage CursorBuffer, int curx, int cury, int screen_width, int screen_height, int s) {
	int sx, sy, ex, ey;

	sx = curx - s;					   // horizontal 
	if (sx < 0) {
		sx = 0;
	}
	ex = curx + s;
	if (ex > screen_width) {
		ex = screen_width;
	}
	cur_sx = sx;
	cur_w = ex - sx;

	sy = cury - s;					   // vertical 
	if (sy < 0) {
		sy = 0;
	}
	ey = cury + s;
	if (ey > screen_height) {
		ey = screen_height;
	}
	cur_sy = sy;
	cur_h = ey - sy;

	vgGetPixels(CursorBuffer, 0, 0, cur_sx, cur_sy, cur_w, cur_h);
	cur_saved = cur_w * cur_h;
}

// restoreCursor restores the pixels under the mouse cursor
void restoreCursor(VGImage CursorBuffer) {
	if (cur_saved != 0) {
		vgSetPixels(cur_sx, cur_sy, CursorBuffer, 0, 0, cur_w, cur_h);
	}
}

// mouseinit starts the mouse event thread
int mouseinit(int w, int h) {
	pthread_t inputThread;
	mouse.max_x = w;
	mouse.max_y = h;
	return pthread_create(&inputThread, NULL, &eventThread, NULL);
}

unsigned int initOpenVG(int width, int height) {
	vg_init(width,height);
	return 1;
}

void destroyOpenVG(void) {
  eglSwapBuffers(state->display, state->surface);
  eglMakeCurrent(state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(state->display, state->surface);
  eglDestroyContext(state->display, state->context);
  eglTerminate(state->display);
}

void vgResizeSurfaceSH(VGint width, VGint height) {}

static int resizeWindow(int w,int h) {}

void swapBuffers(void) {
  eglSwapBuffers(state->display, state->surface);
}

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

void initWindowSize(int x, int y, unsigned int w, unsigned int h) {
	init_x = x;
	init_y = y;
	init_w = w;
	init_h = h;
}

// init sets the system to its initial state
void init(int *w, int *h) {
	bcm_host_init();
	memset(state, 0, sizeof(*state));
	state->window_x = init_x;
	state->window_y = init_y;
	state->window_width = init_w;
	state->window_height = init_h;
	oglinit(state);
	*w = state->window_width;
	*h = state->window_height;
}

void app_close() {
  resetTermios();
  lua_close(L);
  destroyOpenVG();
  killWindow();
  exit(EXIT_SUCCESS);
}

int main(int argc, const char * argv[]) {
	int w, h;
//    initWindowSize(100,100,640,480);
    init(&w, &h);
     if( argc >= 2 )
        strcpy(scriptname,argv[1]);
     else
        strcpy(scriptname,"main.lua");
    
    // initialize AmanithVG
    if (!initOpenVG(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT)) {
        killWindow();
        return EXIT_FAILURE;
    }
    
    lua_init(argc,argv);
    
    if (mouseinit(width, height) != 0) {
		fprintf(stderr, "Unable to initialize the mouse\n");
		exit(1);
	}
    time0 = getTimeMS();
    framesCounter = 0;
    done = displayHelp = displayAbout = 0;
    // main loop
    char ch;
    initTermios(0); //oldChars, newChars);
    while (!done) {
		 //sleep_ms(1000/frameRate);
        // process keyboard and mouse events
        // check if some information text must be displayed
            // draw scene
           drawScene();
 //           drawScene(surfaceWidth, surfaceHeight);
            swapBuffers();
            ch = getchar();
            if (ch=='q') break;
    }
    app_close();
}
