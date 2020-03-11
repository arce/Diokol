/*
--
-- diokol.h
--
-- Döiköl Interactive Graphics Environment
--
-- Copyright (c) 2017-2019 Armando Arce - armando.arce@gmail.com
--
-- This library is free software; you can redistribute it and/or modify
-- it under the terms of the MIT license. See LICENSE for details.
--
*/

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#ifdef __linux__
#include <stb/stb_math.h>
#else
#include <math.h>
#endif

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <VG/openvg.h>
#include <VG/vgu.h>
#include <VG/shFont.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846f
#endif

#ifndef __APPLE__
typedef int bool;
#endif
#define true 1
#define false 0
#define NIL -1

#define PATH_SIZE 32
#define MAX_SEGMENTS 1000

#define TRANSFORM 0
#define STROKE_ATTR 1
#define STROKE_COLOR 0
#define FILL_COLOR 1

#define STROKE_WEIGHT 0
#define STROKE_CAP 1
#define STROKE_JOIN 2
#define TRANSFORM 3

#define ELLIPSE 0
#define RECT 1
#define IMAGE 2
#define SHAPE 3
#define TEXT 4

#define P5_NONE -1
#define P5_OPEN 0
#define P5_CLOSE VG_CLOSE_PATH
#define P5_CENTER 2
#define P5_CORNER 3
#define P5_CORNERS 4
#define P5_RADIUS 5
#define P5_LEFT 6
#define P5_RIGHT 7
#define P5_TOP 8
#define P5_BOTTOM 9
#define P5_CHORD 10
#define P5_PIE 11
#define P5_SQUARE VG_CAP_BUTT
#define P5_PROJECT VG_CAP_SQUARE
#define P5_ROUND VG_JOIN_ROUND
#define P5_MITER VG_JOIN_MITER
#define P5_BEVEL VG_JOIN_BEVEL
#define P5_BASELINE 17
#define P5_MIDDLE 18
#define P5_HORIZONTAL 19
#define P5_VERTICAL 20

#define P5_POINTS 25
#define P5_LINES 26
#define P5_TRIANGLES 27
#define P5_TRIANGLE_FAN 28
#define P5_TRIANGLE_STRIP 29
#define P5_QUADS 30
#define P5_QUAD_STRIP 31
#define P5_POLYLINE 32
#define P5_RECT 33
#define P5_CIRCLE 34
#define P5_ARC 35
#define P5_LINE 36
#define P5_POINT 37
#define P5_CUBIC_TO 38

#define P5_PI 3.14159265358979323846f
#define P5_TWO_PI 6.28318530718f
#define P5_HALF_PI 1.57079632679f
#define P5_QUARTER_PI 0.78539816339f

#define P5_BACKSPACE 100
#define P5_TAB 101
#define P5_ENTER 102
#define P5_RETURN 103
#define P5_ESC 104
#define P5_DELETE 105

#define MOUSE_MOVED 0
#define MOUSE_DRAGGED 1
#define MOUSE_PRESSED 2
#define MOUSE_RELEASED 3
#define MOUSE_CLICKED 4
#define KEY_PRESSED 5
#define KEY_RELEASED 6
#define WINDOW_RESIZED 7

#define _deg(angleRadians) (angleRadians * 180.0 / M_PI)
#define _rad(angleDegrees) (angleDegrees / 180.0 * M_PI)

int error;

VGPaint strokePaint;
VGPaint fillPaint;

VGPath shape_path;
VGPath paths[PATH_SIZE];
VGPath shapePaths[255];

int mode[5] = {P5_CENTER,P5_CORNER,0,0,0}; // [0] = ellipse, [1] = rect, [2] = image, [3] = shape, [4] = text
//int style[5] = {0,0,1,0xFF00FF00,0xFF000000}; // [0] = strokeJoin, [1] = strokeCap, [2] = strokeWeight, [3] = strokeColor, [4] = fillColor

VGImage images[100];
int iWidth[100];
int iHeight[100];
VGFont fonts[100];
int fHeight[100];
int fSize[100];
int imageCount = 0;
int fontCount = 1;
int pathId=0;
int imageId=0;

union StyleUnion {
  VGuint attr[2];
  int64_t data;
};

union PropsUnion {
  VGshort attr[4];
  int64_t data;
};

union StyleUnion style;

union PropsUnion props;

VGfloat transform[9];
VGfloat backup[9];

union StyleUnion pathStyle[PATH_SIZE];

union PropsUnion pathProps[PATH_SIZE];

VGfloat pathTransform[PATH_SIZE][9];

typedef struct Matrix {
    VGfloat data[9];
    struct Matrix *next;
} Matrix;

struct Matrix *mtrx_stack = NULL;

Matrix *mtrx_new() {
    Matrix *mtrx;
    mtrx = malloc(sizeof(Matrix));
    memset(mtrx,0,sizeof(Matrix));
    mtrx->next = NULL;
    return mtrx;
}

char *eventArray[] = {"mouseMoved","mouseDragged","mousePressed","mouseReleased","mouseClicked","keyPressed","keyReleased","windowResized"};

bool isEvent[] = {false,false,false,false,false,false,false,false};

bool eventFunc[] = {true,true,true,true,true,true,true,true};

bool event[] = {false,false,false,false,false,false,false,false};

double mouseX=0, mouseY=0;
double pmouseX=0, pmouseY=0;
double draggX=0, draggY=0;
int eventType = NIL;
int keyCode = 0;
char keyPress[2] = "X\0";
int mouseButton;

bool done=false;
bool loop=true;
bool initialized=false;
int frameCount = 0;

VGint width=640, height=480;
char scriptname[256];
int frameRate = 60;

int32_t clearColor = 0xAAAAAAFF;

lua_State *L;

void bail(lua_State *L, int error, char *msg){
  printf("\nFATAL ERROR:\n  %s: %s\n\n",msg, lua_tostring(L, -1));
  exit(1);
}

static void vg_call(char *fn_name);

static void app_close();

static int resizeWindow(int,int);
		
static void getArrColor(VGfloat* arr, VGuint color) {
  arr[0] = ((color >> 24) & 0xFF)/255.0;
  arr[1] = ((color >> 16) & 0xFF)/255.0;
  arr[2] = ((color >> 8) & 0xFF)/255.0;
  arr[3] = ((color) & 0xFF)/255.0;
}

// Structure:
// exit()
// loop()
// noLoop()
// popStyle() -- todo
// pushStyle() -- todo
// redraw()
// setTitle() -- todo
// time() -- Diököl extension

static int P5_Exit(lua_State *L) {
    done = true;
    app_close();
}

static int P5_Loop(lua_State *L) {
    loop = true;
    return 0;
}

static int P5_NoLoop(lua_State *L) {
    loop = false;
    return 0;
}

static int P5_Redraw(lua_State *L) {
  vg_call("draw");
  return 0;
}


static int P5_Time(lua_State *L) {
    struct timeval tv;
    double t;
    gettimeofday(&tv,NULL);
    t = (tv.tv_sec*1000)+(tv.tv_usec/1000);
    lua_pushnumber(L,t);
    return 1;
}

// Environment:
// frameCount()
// frameRate()
// fullScreen() -- todo
// height()
// size()
// width()

static int P5_FrameCount(lua_State *L) {
    lua_pushnumber(L,frameCount);
    return 1;
}

static int P5_FrameRate(lua_State *L) {
    frameRate = luaL_checkint(L, 1);
    return 0;
}

static int P5_Height(lua_State *L) {
    lua_pushnumber(L, height);
    return 1;
}

static int P5_Size(lua_State *L) {
    width = luaL_checkint(L, 1);
    height = luaL_checkint(L, 2);
    resizeWindow(width,height);
    vgResizeSurfaceSH(width,height);
    return 0;
}

static int P5_Width(lua_State *L) {
    lua_pushnumber(L, width);
    return 1;
}

// 2D Primitives:
// arc()
// circle()
// ellipse()
// line ()
// point()
// quad()
// rect()
// square()
// triangle

static void flushPathByIndex(int index) {
  if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)==0) return;
  VGfloat RGBA[4];
  if (pathProps[index].attr[TRANSFORM]!=0) {
    vgSeti(VG_MATRIX_MODE,VG_MATRIX_PATH_USER_TO_SURFACE);
    vgGetMatrix(backup);
    vgLoadMatrix(pathTransform[index]);
  }
  if (pathProps[index].attr[STROKE_WEIGHT]!=0)
    vgSetf(VG_STROKE_LINE_WIDTH, pathProps[index].attr[STROKE_WEIGHT]);
  if (pathProps[index].attr[STROKE_CAP]!=0)
    vgSeti(VG_STROKE_CAP_STYLE, pathProps[index].attr[STROKE_CAP]);
  if (pathProps[index].attr[STROKE_JOIN]!=0)
    vgSeti(VG_STROKE_JOIN_STYLE, pathProps[index].attr[STROKE_JOIN]);
  if (pathStyle[index].attr[FILL_COLOR]!=0) {
	getArrColor(RGBA,pathStyle[index].attr[FILL_COLOR]);
	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, RGBA);
    vgSetPaint(fillPaint, VG_FILL_PATH);
    vgDrawPath(paths[index], VG_FILL_PATH);
  }
  if (pathStyle[index].attr[STROKE_COLOR]!=0) {
	getArrColor(RGBA,pathStyle[index].attr[STROKE_COLOR]);
	vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, RGBA);
    vgSetPaint(strokePaint, VG_STROKE_PATH);
    vgDrawPath(paths[index], VG_STROKE_PATH);
  }
  if (pathProps[index].attr[TRANSFORM]!=0)
    vgLoadMatrix(backup);
  vgClearPath(paths[index], VG_PATH_CAPABILITY_APPEND_TO);
}

int next=0;

static int findIndex() {
  int i = 0;
  for (; i<PATH_SIZE; i++) {
    if ((pathStyle[i].data==0) ||
        ((pathStyle[i].data==style.data) &&
         (pathProps[i].data==props.data))) {
      if ((pathProps[i].attr[TRANSFORM]!=0) && (memcmp(pathTransform[i],transform,sizeof(transform))!=0))
          continue;
      pathStyle[i].data = style.data;
      pathProps[i].data = props.data;
      if (props.attr[TRANSFORM]!=0)
        memcpy(pathTransform[i],transform,sizeof(transform));
      return i;
    }
  }
  flushPathByIndex(next);
  pathStyle[next].data = style.data;
  pathProps[next].data = props.data;
  if (props.attr[TRANSFORM]!=0)
    memcpy(pathTransform[next],transform,sizeof(transform));
  next++;
  next = (next==PATH_SIZE)?0:next;
  return next;
}

static int P5_Arc(lua_State *L) {
    VGUArcType arcType = VGU_ARC_PIE;
    VGfloat x,y,a,b,start,stop,type;
    x = luaL_checknumber(L, 1);
    y = luaL_checknumber(L, 2);
    a = luaL_checknumber(L, 3);
    b = luaL_checknumber(L, 4);
    start = _deg(luaL_checknumber(L, 5));
    stop = _deg(luaL_checknumber(L, 6));
    if (lua_gettop(L) == 7)
        type = luaL_checkint(L, 7);
    else
        type = 0;
    if (type == P5_PIE)
        arcType = VGU_ARC_PIE;
    else if (type == P5_CHORD)
        arcType = VGU_ARC_CHORD;
    else if (type == P5_OPEN)
        arcType = VGU_ARC_OPEN;
    int index = findIndex();
    
    switch (mode[ELLIPSE]) {
      case P5_CORNERS:
        vguArc(paths[index],(x+a)/2,(y+b)/2, a-x, b-y,start,stop-start,arcType);
        break;
      case P5_CENTER:
        vguArc(paths[index],x,y,a,b,start,stop-start,arcType);
        break;
      case P5_RADIUS:
        vguArc(paths[index],x,y,a*2,b*2,start,stop-start,arcType);
        break;
      case P5_CORNER:
        vguArc(paths[index],x+a/2,y+b/2,a,b,start,stop-start,arcType);
        break;
    }
    if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
        flushPathByIndex(index);
    return 0;
}

static int _Ellipse(VGfloat a, VGfloat b, VGfloat c, VGfloat d) {
    switch (mode[ELLIPSE]) {
        case P5_CORNER:
            a = a + c/2;
            b = b + d/2;
            break;
        case P5_CORNERS:
            a = (a + c)/2 ;
            b = (b + d)/2;
            c = (c - a)*2;
            d = (d - b)*2;
            break;
        case P5_RADIUS:
            c = c*2;
            d = d*2;
            break;
    }
    
    int index = findIndex();
    vguEllipse(paths[index],a,b,c,d);
    if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
        flushPathByIndex(index);
    return 0;
}

static int P5_Circle(lua_State *L) {
    VGfloat a = luaL_checknumber(L, 1);
    VGfloat b = luaL_checknumber(L, 2);
    VGfloat c = luaL_checknumber(L, 3);
    
    return _Ellipse(a,b,c,c);
}

static int P5_Ellipse(lua_State *L) {
    
    VGfloat a = luaL_checknumber(L, 1);
    VGfloat b = luaL_checknumber(L, 2);
    VGfloat c = luaL_checknumber(L, 3);
    VGfloat d = luaL_checknumber(L, 4);
    
    return _Ellipse(a,b,c,d);
}

static int P5_Line(lua_State *L) {
    
    VGfloat a = luaL_checknumber(L, 1);
    VGfloat b = luaL_checknumber(L, 2);
    VGfloat c = luaL_checknumber(L, 3);
    VGfloat d = luaL_checknumber(L, 4);
    
    int index = findIndex();
    vguLine(paths[index],a,b,c,d);
    if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
        flushPathByIndex(index);
    return 0;
}

static int P5_Point(lua_State *L) {
    VGfloat x = luaL_checknumber(L, 1);
    VGfloat y = luaL_checknumber(L, 2);
    
    int index = findIndex();
    vguEllipse(paths[index],x-2,y-2,4,4);
    if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
        flushPathByIndex(index);
}

static int P5_Quad(lua_State *L) {
    const VGfloat coords[8] = {
        luaL_checknumber(L, 1),luaL_checknumber(L, 2),
        luaL_checknumber(L, 3),luaL_checknumber(L, 4),
        luaL_checknumber(L, 5),luaL_checknumber(L, 6),
        luaL_checknumber(L, 7),luaL_checknumber(L, 8)
    };
    int index = findIndex();
    vguPolygon(paths[index],coords,8,true);
    if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
        flushPathByIndex(index);
}

static int _RoundRect(lua_State *L) {
    
    VGfloat x,y,a,b,r1,r2;
    x = luaL_checknumber(L, 1);
    y = luaL_checknumber(L, 2);
    a = luaL_checknumber(L, 3);
    b = luaL_checknumber(L, 4);
    r1 = r2 = luaL_checknumber(L, 5);
    if (lua_gettop(L)==6)
        r2 = luaL_checknumber(L, 6);
    
    int index = findIndex();
    switch (mode[RECT]) {
        case P5_CORNER:
            vguRoundRect(paths[index], x, y, a, b,r1,r2);
            break;
        case P5_CENTER:
            vguRoundRect(paths[index], x-a/2, y-b/2, a, b,r1,r2);
            break;
        case P5_RADIUS:
            vguRoundRect(paths[index], x-a/2, y-b/2, a/2, b/2,r1,r2);
            break;
        case P5_CORNERS:
            vguRoundRect(paths[index], x, y, x+a, y+b,r1,r2);
            break;
    }
    
    if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
        flushPathByIndex(index);
    return 0;
}

static int P5_Rect(lua_State *L) {
  if (lua_gettop(L)==5)
    return _RoundRect(L);
    
  VGfloat a = luaL_checknumber(L, 1);
  VGfloat b = luaL_checknumber(L, 2);
  VGfloat c = luaL_checknumber(L, 3);
  VGfloat d = luaL_checknumber(L, 4);
  
  switch (mode[RECT]) {
    case P5_CORNERS:
	  c =  c - a;
	  d =  d - b;
      break;
    case P5_CENTER:
	  a = a - c/2;
	  b = b - d/2;
      break;
    case P5_RADIUS:
	  a = a - c;
	  b = b - d;
	  c = c*2;
	  d = d*2;
      break;
  }
  
  int index = findIndex();
  vguRect(paths[index],a,b,c,d);
  if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
    flushPathByIndex(index);
  return 0;
}

static int P5_Square(lua_State *L) {
    const VGfloat coords[8] = {
        luaL_checknumber(L, 1),luaL_checknumber(L, 2),
        luaL_checknumber(L, 3),luaL_checknumber(L, 4),
        luaL_checknumber(L, 5),luaL_checknumber(L, 6)
    };
    int index = findIndex();
    vguPolygon(paths[index],coords,6,true);
    if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
        flushPathByIndex(index);
}

static int P5_Triangle(lua_State *L) {
  const VGfloat coords[8] = {
        luaL_checknumber(L, 1),luaL_checknumber(L, 2),
        luaL_checknumber(L, 3),luaL_checknumber(L, 4),
        luaL_checknumber(L, 5),luaL_checknumber(L, 6)
  };
  int index = findIndex();
  vguPolygon(paths[index],coords,6,true);
  if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>MAX_SEGMENTS-10)
    flushPathByIndex(index);
}

// Attributes:
// ellipseMode()
// rectMode()
// strokeCap()
// strokeJoin()
// strokeWeight()

static int P5_EllipseMode(lua_State *L) {
    mode[ELLIPSE] = luaL_checkint(L, 1);
    return 0;
}

static int P5_RectMode(lua_State *L) {
    mode[RECT] = luaL_checkint(L, 1);
    return 0;
}

static int P5_StrokeCap(lua_State *L) {
    props.attr[STROKE_CAP] = luaL_checkint(L, 1);
    return 0;
}

static int P5_StrokeJoin(lua_State *L) {
    props.attr[STROKE_JOIN] = luaL_checkint(L, 1);
    return 0;
}

static int P5_StrokeWeight(lua_State *L) {
    props.attr[STROKE_WEIGHT] = luaL_checkint(L, 1);
}

// Vertex commands:
// beginContour() -- todo
// beginShape()
// bezierVertex()
// curveVertex() -- todo
// endShape()
// quadraticVertex()
// vertex()

int kindShape;
int pathSize;

static int P5_BeginShape(lua_State *L) {
    if (lua_gettop(L)==1)
        kindShape = luaL_checknumber(L, 1);
    else
        kindShape = P5_POLYLINE;
    vgClearPath(shape_path,VG_PATH_CAPABILITY_APPEND_FROM | VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY);
    pathSize = 0;
    return 0;
}

static int P5_BezierVertex(lua_State *L) {
    VGubyte seg = VG_CUBIC_TO;
    const VGfloat coords[6] = {
        luaL_checknumber(L, 1), luaL_checknumber(L, 2),
        luaL_checknumber(L, 3), luaL_checknumber(L, 4),
        luaL_checknumber(L, 5), luaL_checknumber(L, 6)
    };
    vgAppendPathData(shape_path, 1, &seg, coords);
    return 0;
}

static int P5_CurveVertex(lua_State *L) {
    return 0;
}

static int P5_EndShape(lua_State *L) {
    VGubyte seg = -1;
    if (lua_gettop(L)==1) {
        seg = VG_CLOSE_PATH;
        VGfloat data = 0.0f;
        vgAppendPathData(shape_path, 1, &seg, &data);
    }
    if (seg==VG_CLOSE_PATH) _FillPath(shape_path);
    _StrokePath(shape_path);
    _EventPath(shape_path);
}

static int P5_QuadraticVertex(lua_State *L) {
    VGubyte seg = VG_QUAD_TO;
    const VGfloat coords[4] = {
        luaL_checknumber(L, 1), luaL_checknumber(L, 2),
        luaL_checknumber(L, 3), luaL_checknumber(L, 4)
    };
    vgAppendPathData(shape_path, 1, &seg, coords);
    return 0;
}

static int P5_Vertex(lua_State *L) {
    VGubyte seg;
    int closePath = 0;
    int points = 0;
    const VGfloat coords[2] = {
        luaL_checknumber(L, 1), luaL_checknumber(L, 2)
    };
    switch (kindShape) {
        case P5_POINTS:
            seg = VG_MOVE_TO;
            points = 1;
            break;
        case P5_POLYLINE:
            if (pathSize == 0)
                seg = VG_MOVE_TO;
            else
                seg = VG_LINE_TO;
            break;
        case P5_LINES:
            if (pathSize%2 == 0)
                seg = VG_MOVE_TO;
            else
                seg = VG_LINE_TO;
            break;
        case P5_TRIANGLES:
            if (pathSize%3 == 0)
                seg = VG_MOVE_TO;
            else {
                seg = VG_LINE_TO;
                if (pathSize%3 == 2) closePath = 1;
            }
            break;
        case P5_QUADS:
            if (pathSize%4 == 0)
                seg = VG_MOVE_TO;
            else {
                seg = VG_LINE_TO;
                if (pathSize%4 == 3) closePath = 1;
            }
            break;
    }
    pathSize++;
    vgAppendPathData(shape_path, 1, &seg, coords);
    if (points) {
        seg = VG_LINE_TO;
        vgAppendPathData(shape_path, 1, &seg, coords);
    }
    if (closePath) {
        seg = VG_CLOSE_PATH;
        vgAppendPathData(shape_path, 1, &seg, coords);
    }
    return 0;
}

// Loading & Displaying:
// saveShape() - Diököl extension
// shape()
// shapeMode()

static int P5_SaveShape(lua_State *L) {
    VGubyte seg;
    if (lua_gettop(L)==1) {
        seg = luaL_checknumber(L, 1);
        VGfloat data = 0.0f;
        vgAppendPathData(shape_path, 1, &seg, &data);
    }
    pathId++;
    shapePaths[pathId]=vgCreatePath(VG_PATH_FORMAT_STANDARD,VG_PATH_DATATYPE_F,1.0f,0.0f,0,0,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
    vgAppendPath(shapePaths[pathId],shape_path);
    lua_pushnumber(L, pathId);
    return 1;
}

static int P5_Shape(lua_State *L) {
  int pathId = luaL_checkint(L, 1);
  float x = luaL_checknumber(L,2);
  float y = luaL_checknumber(L,3);
  double sx = 1;
  double sy = 1;
  if (lua_gettop(L) == 5) {
    sx = luaL_checknumber(L,4);
    sy = luaL_checknumber(L,5);
  }
 // flushPath(shapePaths[pathId]);
}

static int P5_ShapeMode(lua_State *L) {
    mode[SHAPE] = luaL_checkint(L, 1);
    return 0;
}

// Input:
// mouseButton
// mouseClicked()
// mouseDragged()
// mouseMoved()
// mousePressed()
// mousePressed
// mouseReleased()
// mouseWheel()
// mouseX
// mouseY
// pmouseX
// pmouseY


static int P5_MouseButton(lua_State *L) {
    lua_pushnumber(L,mouseButton);
    return 1;
}

static int P5_IsMousePressed(lua_State *L) {
    lua_pushboolean(L,isEvent[MOUSE_PRESSED]);
    return 1;
}

static int Dkl_MousePressed(int xpos, int ypos,int button) {
    pmouseX = mouseX;
    pmouseY = mouseY;
    mouseX = xpos;
    mouseY = ypos;
    if (button)
        mouseButton = P5_RIGHT;
    else
        mouseButton = P5_LEFT;
    eventType = MOUSE_PRESSED;
    isEvent[MOUSE_PRESSED] = true;
    return 0;
}

static int Dkl_MouseReleased(int xpos, int ypos, int button) {
    pmouseX = mouseX;
    pmouseY = mouseY;
    mouseX = xpos;
    mouseY = ypos;
    if (button)
        mouseButton = P5_RIGHT;
    else
        mouseButton = P5_LEFT;
    eventType = MOUSE_RELEASED;
    isEvent[MOUSE_RELEASED] = true;
    return 0;
}

static int Dkl_MouseMoved(int xpos, int ypos, int dragged) {
    pmouseX = mouseX;
    pmouseY = mouseY;
    mouseX = xpos;
    mouseY = ypos;
    if (dragged) {
        eventType = MOUSE_DRAGGED;
        isEvent[MOUSE_DRAGGED] = true;
    } else {
        eventType = MOUSE_MOVED;
        isEvent[MOUSE_MOVED] = true;
    }
    return 0;
}

static void Dkl_ProcessEvent(int eventType) {
    lua_getglobal(L,eventArray[eventType]);
    if (eventType == WINDOW_RESIZED) {
        lua_pushnumber(L, width);
        lua_pushnumber(L, height);
    } else if (eventType == KEY_PRESSED || eventType == KEY_RELEASED) {
        lua_pushstring(L, keyPress);
        lua_pushnumber(L, keyCode);
    } else {
        lua_pushnumber(L, mouseX);
        lua_pushnumber(L, mouseY);
    }
    if (lua_pcall(L, 2, 0, 0)!=0)
        eventFunc[eventType] = false;
    if (eventType == MOUSE_RELEASED && eventFunc[MOUSE_CLICKED]) {
        lua_getglobal(L,eventArray[MOUSE_CLICKED]);
        lua_pushnumber(L, mouseX);
        lua_pushnumber(L, mouseY);
        if (lua_pcall(L, 2, 0, 0)!=0)
            eventFunc[MOUSE_CLICKED] = false;
    }
}

// Keyboard:
// key
// keyCode
// keyPressed()
// keyPressed
// keyReleased()
// keyTyped()

static int Dkl_KeyPressed(char key, int code) {
    keyPress[0] = key;
    keyCode = code;
    eventType = KEY_PRESSED;
    isEvent[KEY_PRESSED] = true;
    return 0;
}

static int Dkl_KeyReleased(char key, int code) {
    keyCode = code;
    eventType = KEY_RELEASED;
    isEvent[KEY_RELEASED] = true;
    return 0;
}

// Transform


static int P5_PopMatrix(lua_State *L) {
    Matrix* temp = mtrx_stack;
    mtrx_stack = mtrx_stack->next;
    
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadMatrix(temp->data);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgLoadMatrix(temp->data);
    return 0;
}

static int P5_PrintMatrix(lua_State *L) {
    Matrix* temp = mtrx_stack;
    int i,j;
    for (i=0;i<3;i++) {
        for (j=0; j<3; j++)
            printf("%f ",temp->data[i*3+j]);
        printf("\n");
    }
    return 0;
}

static int P5_PushMatrix(lua_State *L) {
    Matrix* temp;
    temp = mtrx_new();
    
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgGetMatrix(temp->data);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgGetMatrix(temp->data);
    temp->next = mtrx_stack;
    mtrx_stack = temp;
    return 0;
}

static int P5_ResetMatrix(lua_State *L) {
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgLoadIdentity();
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgLoadIdentity();
    return 0;
}

static int P5_Rotate(lua_State *L) {
    float angle = _deg(luaL_checknumber(L, 1));
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgRotate(angle);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgRotate(angle);
    return 0;
}

static int P5_Scale(lua_State *L) {
    float sx = luaL_checknumber(L, 1);
    float sy = sx;
    if (lua_gettop(L)==2)
        sy = luaL_checknumber(L, 2);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgScale(sx,sy);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgScale(sx,sy);
    return 0;
}

static int P5_ShearX(lua_State *L) {
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgShear(luaL_checknumber(L, 1),0.0f);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgShear(luaL_checknumber(L, 1),0.0f);
    return 0;
}

static int P5_ShearY(lua_State *L) {
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgShear(0.0f,luaL_checknumber(L, 1));
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgShear(0.0f,luaL_checknumber(L, 1));
    return 0;
}

static int P5_Translate(lua_State *L) {
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
    vgTranslate(luaL_checknumber(L, 1),luaL_checknumber(L, 2));
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgTranslate(luaL_checknumber(L, 1),luaL_checknumber(L, 2));
    return 0;
}

// Color Setting:
// background()
// clear()
// fill()
// noFill()
// noStroke()
// stroke()

static VGuint Color(lua_State *L) {
    VGuint r,g,b,a;
    if (lua_gettop(L)==1) {
        r = g = b = luaL_checkint(L, 1);
        if (r>255) return r;
        a = 255;
    } else if (lua_gettop(L)==2) {
        r = g = b = luaL_checkint(L, 1);
        a = luaL_checkint(L, 2);
    } else if (lua_gettop(L)==3) {
        r = luaL_checkint(L, 1);
        g = luaL_checkint(L, 2);
        b = luaL_checkint(L, 3);
        a = 255;
    } else if (lua_gettop(L)==4) {
        r = luaL_checkint(L, 1);
        g = luaL_checkint(L, 2);
        b = luaL_checkint(L, 3);
        a = luaL_checkint(L, 4);
    }
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) +
    ((b & 0xff) << 8) + (a & 0xff);
}

static int P5_Background(lua_State *L) {
    VGfloat RGBA[4];
    getArrColor(RGBA,Color(L));
    vgSetfv(VG_CLEAR_COLOR, 4, RGBA);
    vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgClear(0,0,width,height);
    return 0;
}

static int P5_Fill(lua_State *L) {
    style.attr[FILL_COLOR] = Color(L);
    return 0;
}

static int P5_NoFill(lua_State *L) {
    style.attr[FILL_COLOR] = 0;
    return 0;
}

static int P5_NoStroke(lua_State *L) {
    style.attr[STROKE_COLOR] = 0;
    return 0;
}

static int P5_Stroke(lua_State *L) {
    style.attr[STROKE_COLOR] = Color(L);
    return 0;
}

static void flushBuffers() {
  for (int i=0; i<PATH_SIZE; i++)
    flushPathByIndex(i);
}

static int P5_Flush(lua_State *L) {
  flushBuffers();
  return 0;
}

static int P5_ImageMode(lua_State *L) {
  mode[IMAGE] = luaL_checkint(L, 1);
  return 0;
}

static int P5_TextMode(lua_State *L) {
  mode[TEXT] = luaL_checkint(L, 1);
  return 0;
}

static int P5_Smooth(lua_State *L) {
  return 0;
}

static int P5_IsKeyPressed(lua_State *L) {
  lua_pushboolean(L,isEvent[KEY_PRESSED]);
  return 1;
}

static int P5_Color(lua_State *L) {
  lua_pushnumber(L,Color(L));
  return 1;
}

static int P5_Key(lua_State *L) {
  lua_pushstring(L,keyPress);
  return 1;
}

static int P5_KeyCode(lua_State *L) {
  lua_pushnumber(L,keyCode);
  return 1;
}

// OpenVG functions

void vg_init(int w,int h) {
	VGfloat RGBA[4];
	
	width = w; height = h;
	strokePaint = vgCreatePaint();
	vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

	fillPaint = vgCreatePaint();
	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	
    style.data = 0;
    
	for (int i=0; i<PATH_SIZE; i++) {
	  paths[i] = vgCreatePath(VG_PATH_FORMAT_STANDARD,VG_PATH_DATATYPE_F,1.0f,0.0f,MAX_SEGMENTS,6,VG_PATH_CAPABILITY_APPEND_TO);
	  pathStyle[i].data = 0;
	}
    
    getArrColor(RGBA,clearColor);
	vgSetfv(VG_CLEAR_COLOR, 4, RGBA);
	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgClear(0,0,480,640);
	vgSeti(VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_FASTER);
}

void vg_call(char *fn_name) {
	lua_getglobal(L,fn_name);

	vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
	vgLoadIdentity();
	vgScale(1,-1);
	vgTranslate(0,-height);

	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgLoadIdentity();
	vgScale(1,-1);
	vgTranslate(0,-height);

	if ((error = lua_pcall(L, 0, 0, 0)))			
		bail(L, error, "Error running script\n");
    frameCount++;
}

void vg_resize(int w, int h) {
	width = w;
	height = h;
	vgResizeSurfaceSH(w,h);

	vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
	vgLoadIdentity();
	vgScale(1,-1);
	vgTranslate(0,-height);

	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgLoadIdentity();
	vgScale(1,-1);
	vgTranslate(0,-height);
}

static int _resetEvents() {
    isEvent[MOUSE_PRESSED] = false;
    isEvent[MOUSE_RELEASED] = false;
    isEvent[MOUSE_MOVED] = false;
    isEvent[MOUSE_DRAGGED] = false;
    isEvent[MOUSE_CLICKED] = false;
    isEvent[KEY_PRESSED] = false;
    isEvent[KEY_RELEASED] = false;
}

int drawScene() {
    if (eventType!=NIL)
        Dkl_ProcessEvent(eventType);
    if (loop&&initialized) {
        vg_call("draw");
        flushBuffers();
    }
    eventType = NIL;
    _resetEvents();
}

int luaRegisterAPI(int argc, const char * argv[]) {
    
    if (argc>=2)
      lua_newtable(L);
    
    for (int i=2; i < argc; i++) {
        lua_pushnumber(L, i-1);
        lua_pushnumber(L, atoi(argv[i]));
        lua_rawset(L, -3);
    }
    
    if (argc>=2)
      lua_setglobal(L,"args");
	
    lua_pushnumber(L,P5_NONE);
    lua_setglobal(L,"NONE");
    
    lua_pushnumber(L,P5_OPEN);
    lua_setglobal(L,"OPEN");
    
    lua_pushnumber(L,P5_CLOSE);
    lua_setglobal(L,"CLOSE");
    
    lua_pushnumber(L,P5_CENTER);
    lua_setglobal(L,"CENTER");
    
    lua_pushnumber(L,P5_CORNER);
    lua_setglobal(L,"CORNER");
    
    lua_pushnumber(L,P5_CORNERS);
    lua_setglobal(L,"CORNERS");
    
    lua_pushnumber(L,P5_RADIUS);
    lua_setglobal(L,"RADIUS");
    
    lua_pushnumber(L,P5_LEFT);
    lua_setglobal(L,"LEFT");
    
    lua_pushnumber(L,P5_RIGHT);
    lua_setglobal(L,"RIGHT");
    
    lua_pushnumber(L,P5_TOP);
    lua_setglobal(L,"TOP");
    
    lua_pushnumber(L,P5_BOTTOM);
    lua_setglobal(L,"BOTTOM");
    
    lua_pushnumber(L,P5_CHORD);
    lua_setglobal(L,"CHORD");
    
    lua_pushnumber(L,P5_PIE);
    lua_setglobal(L,"PIE");
    
    lua_pushnumber(L,P5_SQUARE);
    lua_setglobal(L,"SQUARE");
    
    lua_pushnumber(L,P5_PROJECT);
    lua_setglobal(L,"PROJECT");
    
    lua_pushnumber(L,P5_ROUND);
    lua_setglobal(L,"ROUND");
    
    lua_pushnumber(L,P5_MITER);
    lua_setglobal(L,"MITER");
    
    lua_pushnumber(L,P5_BEVEL);
    lua_setglobal(L,"BEVEL");
    
    lua_pushnumber(L,P5_BASELINE);
    lua_setglobal(L,"BASELINE");
    
    lua_pushnumber(L,P5_MIDDLE);
    lua_setglobal(L,"MIDDLE");
    
    lua_pushnumber(L,P5_HORIZONTAL);
    lua_setglobal(L,"HORIZONTAL");
    
    lua_pushnumber(L,P5_VERTICAL);
    lua_setglobal(L,"VERTICAL");
    
    lua_pushnumber(L,P5_PI);
    lua_setglobal(L,"PI");
    
    lua_pushnumber(L,P5_TWO_PI);
    lua_setglobal(L,"TWO_PI");
    
    lua_pushnumber(L,P5_HALF_PI);
    lua_setglobal(L,"HALF_PI");
    
    lua_pushnumber(L,P5_QUARTER_PI);
    lua_setglobal(L,"QUARTER_PI");
    
    lua_pushnumber(L,P5_POINTS);
    lua_setglobal(L,"POINTS");
    
    lua_pushnumber(L,P5_LINES);
    lua_setglobal(L,"LINES");
    
    lua_pushnumber(L,P5_TRIANGLES);
    lua_setglobal(L,"TRIANGLES");
    
    lua_pushnumber(L,P5_QUADS);
    lua_setglobal(L,"QUADS");
    
    lua_pushnumber(L,P5_POLYLINE);
    lua_setglobal(L,"POLYLINE");
    
    lua_pushnumber(L,P5_RECT);
    lua_setglobal(L,"RECT");
    
    lua_pushnumber(L,P5_CIRCLE);
    lua_setglobal(L,"CIRCLE");
    
    lua_pushnumber(L,MOUSE_MOVED);
    lua_setglobal(L,"MOVED");
    
    lua_pushnumber(L,MOUSE_DRAGGED);
    lua_setglobal(L,"DRAGGED");
    
    lua_pushnumber(L,MOUSE_PRESSED);
    lua_setglobal(L,"PRESSED");
    
    lua_pushnumber(L,MOUSE_RELEASED);
    lua_setglobal(L,"RELEASED");
    
    lua_pushnumber(L,MOUSE_CLICKED);
    lua_setglobal(L,"CLICKED");
    
    lua_pushnumber(L,P5_BACKSPACE);
    lua_setglobal(L,"BACKSPACE");
    
    lua_pushnumber(L,P5_TAB);
    lua_setglobal(L,"TAB");
    
    lua_pushnumber(L,P5_ENTER);
    lua_setglobal(L,"ENTER");
    
    lua_pushnumber(L,P5_RETURN);
    lua_setglobal(L,"RETURN");
    
    lua_pushnumber(L,P5_ESC);
    lua_setglobal(L,"ESC");
    
    lua_pushnumber(L,P5_DELETE);
    lua_setglobal(L,"DELETE");

// functions

    lua_pushcfunction(L,P5_FrameRate);
    lua_setglobal(L,"frameRate");
    
    lua_pushcfunction(L,P5_Flush);
	lua_setglobal(L,"flush");
	
	lua_pushcfunction(L,P5_Exit);
	lua_setglobal(L,"exit");
	
	lua_pushcfunction(L,P5_Time);
	lua_setglobal(L,"time");
	  
	lua_pushcfunction(L,P5_Size);
	lua_setglobal(L,"size");
    
    lua_pushcfunction(L,P5_Height);
    lua_setglobal(L,"height");
    
    lua_pushcfunction(L,P5_Width);
    lua_setglobal(L,"width");
	
    lua_pushcfunction(L,P5_Arc);
    lua_setglobal(L,"arc");
    
    lua_pushcfunction(L,P5_Ellipse);
    lua_setglobal(L,"ellipse");
  
    lua_pushcfunction(L,P5_Circle);
    lua_setglobal(L,"circle");
    
    lua_pushcfunction(L,P5_Line);
    lua_setglobal(L,"line");
    
    lua_pushcfunction(L,P5_Rect);
    lua_setglobal(L,"rect");
    
    lua_pushcfunction(L,P5_StrokeCap);
	lua_setglobal(L,"strokeCap");
 
	lua_pushcfunction(L,P5_StrokeJoin);
	lua_setglobal(L,"strokeJoin");

	lua_pushcfunction(L,P5_StrokeWeight);
	lua_setglobal(L,"strokeWeight");
	
	lua_pushcfunction(L,P5_Background);
	lua_setglobal(L,"background");

    lua_pushcfunction(L,P5_Color);
    lua_setglobal(L,"color");
    
	lua_pushcfunction(L,P5_Fill);
	lua_setglobal(L,"fill");

	lua_pushcfunction(L,P5_NoFill);
	lua_setglobal(L,"noFill");

	lua_pushcfunction(L,P5_Stroke);
	lua_setglobal(L,"stroke");
	
	lua_pushcfunction(L,P5_NoStroke);
	lua_setglobal(L,"noStroke");
    
    lua_pushcfunction(L,P5_RectMode);
    lua_setglobal(L,"rectMode");
    
    lua_pushcfunction(L,P5_EllipseMode);
    lua_setglobal(L,"ellipseMode");
    
    lua_pushcfunction(L,P5_TextMode);
    lua_setglobal(L,"textMode");
    
    lua_pushcfunction(L,P5_ImageMode);
    lua_setglobal(L,"imageMode");
    
    lua_pushcfunction(L,P5_ShapeMode);
    lua_setglobal(L,"shapeMode");

    lua_pushcfunction(L,P5_Smooth);
    lua_setglobal(L,"smooth");
    
    lua_pushcfunction(L,P5_PopMatrix);
    lua_setglobal(L,"popMatrix");
    
    lua_pushcfunction(L,P5_PrintMatrix);
    lua_setglobal(L,"printMatrix");
    
    lua_pushcfunction(L,P5_PushMatrix);
    lua_setglobal(L,"pushMatrix");
    
    lua_pushcfunction(L,P5_ResetMatrix);
    lua_setglobal(L,"resetMatrix");

    lua_pushcfunction(L,P5_MouseButton);
    lua_setglobal(L,"mouseButton");
    
    lua_pushcfunction(L,P5_IsMousePressed);
    lua_setglobal(L,"isMousePressed");
    
    lua_pushcfunction(L,P5_IsKeyPressed);
    lua_setglobal(L,"isKeyPressed");
    
    lua_pushcfunction(L,P5_Key);
    lua_setglobal(L,"key");
    
    lua_pushcfunction(L,P5_KeyCode);
    lua_setglobal(L,"keyCode");
    
	return 0;
}

void lua_init(int argc, const char * argv[]) {
  L = luaL_newstate();
  luaL_openlibs(L);
  luaRegisterAPI(argc,argv);
    
  if (luaL_loadfile(L,scriptname))
    bail(L,0,"luaL_loadfile() failed");
  if ((error = lua_pcall(L, 0, 0, 0)))
    bail(L, error, "Error running script\n");
  vg_call("setup");
  initialized=true;
  srand(time(NULL));
}

void lua_destroy() {
  lua_close(L);
}
