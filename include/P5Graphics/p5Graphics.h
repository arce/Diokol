#include <stdbool.h>
#include <stdint.h>

#define P5_NONE -1
#define P5_PIE 0
#define P5_CHORD 1
#define P5_OPEN 2
#define P5_CORNERS 3
#define P5_CENTER 4
#define P5_RADIUS 5
#define P5_CORNER 6
#define P5_MODEL 7
#define P5_SHAPE 8
#define P5_CLOSE 9

#define P5_LEFT 10
#define P5_RIGHT 11
#define P5_TOP 12
#define P5_BOTTOM 13
#define P5_SQUARE 16
#define P5_PROJECT 17
#define P5_ROUND 18
#define P5_MITER 19
#define P5_BEVEL 20
#define P5_BASELINE 17
#define P5_MIDDLE 18
#define P5_HORIZONTAL 19
#define P5_VERTICAL 20

#define P5_BACKSPACE 100
#define P5_TAB 101
#define P5_ENTER 102
#define P5_RETURN 103
#define P5_ESC 104
#define P5_DELETE 105

#define P5_PRESS 9
#define P5_RELEASE 10

typedef int PImage;
typedef int PFont;
typedef int color;

void p5_exit();

void p5_loop();

void p5_noLoop();

void p5_popStyle();

void p5_pushStyle();

void p5_redraw();

double p5_time();

void p5_mainLoop();

void p5_setupFunc(void (*func)());

void p5_drawFunc(void (*func)());

void p5_sizeFunc(void (*func)(int w, int h));

int p5_init(int w, int h);

int p5_frameCount();

int p5_frameRate(int rate);

void p5_fullScreen();

int p5_height();

void p5_noSmooth();

void p5_size(int width, int height);

void p5_smooth();

int p5_width();

void p5_background(int32_t rgba);

void p5_fill(int32_t rgba);

void p5_noFill();

void p5_noStroke();

void p5_stroke(int32_t rgba);

int p5_alpha(int32_t rgba);

int p5_blue(int32_t rgba);

int32_t p5_color(int r, int g, int b, int a);

int p5_green(int32_t rgba);

int32_t p5_lerpColor(int32_t ca, int32_t cb, float t);

int p5_red(int32_t rgba);

#define P5_POINTS 1
#define P5_POLYLINE 2
#define P5_LINES 3
#define P5_TRIANGLES 4
#define P5_QUADS 5
#define P5_RECT 6

void p5_arc(float x, float y, float a, float start, float stop, int type);

void p5_circle(float x, float y, float a);

void p5_ellipse(float x, float y, float a, float b);

void p5_line(float x, float y, float a, float b);

void p5_point(float x, float y);

void p5_quad(float x, float y, float a, float b, float c, float d, float e,
             float f);

void p5_rect(float x, float y, float a, float b);

void p5_square(float x, float y, float a);

void p5_triangle(float x, float y, float a, float b, float c, float d);

void p5_bezier(float x, float y, float a, float b, float c, float d, float e,
               float f);

void p5_curve(float x, float y, float a, float b, float c, float d);

void p5_ellipseMode(int mode);

void p5_rectMode(int mode);

void p5_strokeCap(int cap);

void p5_strokeJoin(int join);

void p5_strokeWeight(float weight);

void p5_strokeMiter(float miter);

void p5_beginShape(int kind);

void p5_bezierVertex(float x, float y, float a, float b, float c, float d);

void p5_curveVertex();

void p5_endShape(int mode);

void p5_quadraticVertex(float x, float y, float w, float h);

void p5_vertex(int x, int y);

int p5_saveShape(int seg);

void p5_shape(int pid, int w, int h);

void p5_shapeMode(int shapeMode);

// p5Input.h

void p5_mouseMovedFunc(void (*func)());

void p5_mouseDraggedFunc(void (*func)());

void p5_mousePressedFunc(void (*func)());

void p5_mouseReleasedFunc(void (*func)());
	
void p5_mouseClickedFunc(void (*func)());

void p5_keyPressedFunc(void (*func)());

void p5_keyReleasedFunc(void (*func)());

void p5_windowResizedFunc(void (*func)(int w, int h));

int p5_mouseX();

int p5_mouseY();

int p5_pmouseX();

int p5_pmouseY();

int p5_mouseButton();

bool p5_isMousePressed();

void p5_mouseEvent(int button, int state);

void p5_motionEvent(int xpos, int ypos);

void p5_passiveMotionEvent(int xpos, int ypos);

void p5_keyPressedEvent(char key, int code);

void p5_keyReleasedEvent(char key, int code);

int p5_isKeyPressed();

int p5_key();

int p5_keyCode();

void p5_popMatrix();

void p5_printMatrix();

void p5_pushMatrix();

void p5_resetMatrix();

void p5_rotate(float angle);

void p5_scale(int sx, int sy);

void p5_shearX(float angle);

void p5_shearY(float angle);

void p5_translate(float dx, float dy);

#define P5_PI 3.14159265358979323846f
#define P5_TWO_PI 6.28318530718f
#define P5_HALF_PI 1.57079632679f
#define P5_QUARTER_PI 0.78539816339f

float p5_constrain(float amt, float low, float high);

float p5_dist(float x1, float y1, float x2, float y2);

float P5_lerp(float a, float b, float f);

float p5_mag(float x, float y);

float p5_map(float value, float start1, float stop1, float start2, float stop2);

float p5_norm(float value, float start1, float stop1);

float p5_degrees(float radians);

float p5_radians(float degrees);

int p5_random(int x);

int p5_createImage(int width, int height);

int p5_imageWidth(int imageId);

int p5_imageHeight(int imageId);

void p5_image(int imageId, int x, int y, int w, int h);

void p5_imageMode(int imageMode);

int p5_importImage(uint8_t *buffer, int len);

int p5_loadImage(char *filename);

void p5_loadPixels(int imageId);

void p5_updatePixels(int imageId, uint8_t **parray);

void p5_clip(int x, int y, int w, int h);

void p5_noClip();

 int p5_createFont(char *filename, int textHeight);

 int p5_loadFont(char *filename, int textHeight);

 void p5_text(char *str, int x, int y);

 void p5_textFont(int id);

 void p5_textAlign(int xalig, int yalig);

 void p5_textLeading(int textLead);

 void p5_textSize(int id);

 int p5_textWidth(char *str);