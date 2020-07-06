#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <GL/glut.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "diokol.h"

int window;

static void displayFunc(void) {

  if (done) glutDestroyWindow(window);
  drawScene();
  glutSwapBuffers();
  // glFlush();
}

static void KeyFunc(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    exit(0);
  }
}

static int resizeWindow(int w,int h) {
    vgResizeSurfaceSH(w, h);
}

void initOpenVG(int w, int h) {
    vgCreateContextSH(w,h);
    vg_init(w,h);
}

void app_close() {
  lua_close(L);
  vgDestroyContextSH();
}

void idleFunc(void) {
  glutPostRedisplay();
}

int main(int argc, char **argv) {

  if( argc >= 2 )
      strcpy(scriptname,argv[1]);
  else
      strcpy(scriptname,"main.lua");

  LEFT_BUTTON = GLUT_LEFT_BUTTON;
  RIGHT_BUTTON = GLUT_RIGHT_BUTTON;
  MIDDLE_BUTTON = GLUT_MIDDLE_BUTTON;
  DOWN = GLUT_DOWN;
  
  glutInit(&argc, argv);

#if defined(__APPLE__) || defined(WIN32)
   glutInitDisplayString("rgba stencil double samples=4");
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA |
                       GLUT_STENCIL | GLUT_MULTISAMPLE);
#else
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA |
                       GLUT_STENCIL | GLUT_MULTISAMPLE);
#endif

  window = glutCreateWindow("Diököl");

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(width, height);

  glutReshapeWindow(width, height);
  
  glutReshapeFunc(resizeWindow);
  glutDisplayFunc(displayFunc);
  glutIdleFunc(idleFunc);
  
  glutKeyboardFunc(keyboardFunc);
  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);
  glutPassiveMotionFunc(passiveMotionFunc);

  initOpenVG(640,480);
  lua_init(argc,argv);
  glutMainLoop();
  app_close();
  return EXIT_SUCCESS;
}
