#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <GL/glut.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "diokol.h"

static void DisplayFunc(void) {

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

void IdleFunc(void) {
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

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Diököl");

  glutReshapeFunc(resizeWindow);
  glutDisplayFunc(DisplayFunc);
  glutIdleFunc(IdleFunc);
  
  glutKeyboardFunc(KeyboardFunc);
  glutMouseFunc(MouseFunc);
  glutMotionFunc(MotionFunc);
  glutPassiveMotionFunc(PassiveMotionFunc);

  initOpenVG(640,480);
  lua_init(argc,argv);
  glutMainLoop();
  app_close();
  return EXIT_SUCCESS;
}
