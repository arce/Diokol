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

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

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

#ifndef M_PI
#    define M_PI 3.14159265358979323846f
#endif

float xA,yA,xB,yB,wB,hB;
int error;

#ifndef __APPLE__
typedef int bool;
#endif
#define true 1
#define false 0

char *eventArray[] = {"mouseMoved","mouseDragged","mousePressed","mouseReleased","mouseClicked","keyPressed","keyReleased","windowResized"};

#define MOUSE_MOVED 0
#define MOUSE_DRAGGED 1
#define MOUSE_PRESSED 2
#define MOUSE_RELEASED 3
#define MOUSE_CLICKED 4
#define KEY_PRESSED 5
#define KEY_RELEASED 6
#define WINDOW_RESIZED 7

// arreglo indicando el evento que sucedió
bool isEvent[] = {false,false,false,false,false,false,false,false};

bool eventFunc[] = {true,true,true,true,true,true,true,true};

// arreglo de eventos a escuchar
bool event[] = {false,false,false,false,false,false,false,false};

double mouseX=0, mouseY=0;
double pmouseX=0, pmouseY=0;
double draggX=0, draggY=0;
bool contains;

VGfloat matrix[9];

#define _deg(angleRadians) (angleRadians * 180.0 / M_PI)
#define _rad(angleDegrees) (angleDegrees / 180.0 * M_PI)

#define _StrokePath(_path) \
	if (!noStroke) vgDrawPath(_path, VG_STROKE_PATH)

#define _FillPath(_path) \
	if (!noFill) vgDrawPath(_path, VG_FILL_PATH)

int p,evtExists,evtCode,evtn;

#define _EventPath(_path) \
	if (noEvent) return 0; \
	if (eventType == -1) return 0; \
	if (event[eventType] == false) return 0; \
	xA = mouseX; yA = height-mouseY; \
	vgPathTransformedBounds(_path,&xB,&yB,&wB,&hB); \
	contains = ((xA >= xB) && (xA <= xB+wB) && (yA >= yB) && (yA <= yB+hB)); \
	if (!contains) return 0; \
	lua_createtable(L, 7, 0); \
	lua_pushnumber(L, eventType); lua_setfield(L, -2, "event");\
	lua_pushnumber(L, mouseX); lua_setfield(L, -2, "mouseX");\
	lua_pushnumber(L, mouseY); lua_setfield(L, -2, "mouseY");\
	lua_pushnumber(L, xB); lua_setfield(L, -2, "minX");\
	lua_pushnumber(L, yB); lua_setfield(L, -2, "minY");\
	lua_pushnumber(L, xB+wB); lua_setfield(L, -2, "maxX");\
	lua_pushnumber(L, yB+hB); lua_setfield(L, -2, "maxY");\
	return 1

#define _SetStroke(color) \
	vgSetParameterfv(strokePaint,VG_PAINT_COLOR,4,color); \
	vgSetPaint(strokePaint, VG_STROKE_PATH)

#define _SetFill(color) \
	vgSetParameterfv(fillPaint,VG_PAINT_COLOR,4,color); \
	vgSetPaint(fillPaint, VG_FILL_PATH)

#define _SetStrokeWeight(w) \
	vgSetf(VG_STROKE_LINE_WIDTH, w)

#define _SetStrokeCap(c) \	
	vgSeti(VG_STROKE_CAP_STYLE, c)

#define _SetStrokeJoin(j) \
	vgSeti(VG_STROKE_JOIN_STYLE, j)
		   
#define _copyColor(a, b) \
	{ int _i; for (_i=0;_i<4;_i++) a[_i] = b[_i]; }

VGfloat blackColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
VGfloat whiteColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
VGfloat clearColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	
const colorSize = 9;

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

VGPaint strokePaint;
VGPaint fillPaint;

VGPath shape_path;
VGPath line_path;
VGPath rect_path;
VGPath point_path;
VGPath ellipse_path;
VGPath bezier_path;
VGPath quad_path;
VGPath triangle_path;
VGPath roundrect_path;
VGPath temp_path;

int kindShape = P5_POLYLINE;

typedef struct Context {
	VGfloat stroke[4];
	VGfloat fill[4];
	VGfloat tint[4];
	int strokeWeight;
	int strokeCap;
	int strokeJoin;
	int imageMode;
	int rectMode;
	int ellipseMode;
	int shapeMode;
	int colorMode;
	int textAlign;
	int textFont;
	int textMode;
	int textSize;
	int textLeading;
	struct Context *next;
} Context;

struct Context *ctx = NULL;

Context *ctx_new() {
	Context *tmp;
	tmp = malloc(sizeof(Context));
	memset(tmp,0,sizeof(Context));

	_copyColor(tmp->stroke,blackColor);
	_copyColor(tmp->fill,whiteColor);
	_copyColor(tmp->tint,whiteColor);
	tmp->strokeWeight = 1;
	tmp->strokeCap = VG_CAP_ROUND;
	tmp->strokeJoin = P5_MITER;
	tmp->imageMode = P5_CORNER;
	tmp->rectMode = P5_CORNER;
	tmp->ellipseMode = P5_CENTER;
	tmp->shapeMode = P5_CORNER;
	tmp->textAlign = P5_RIGHT;
	tmp->textFont = 0;
	tmp->textSize = 1;
	tmp->textLeading = 0;
	tmp->next = NULL;
	return tmp;
}

Context *ctx_clone(Context *src) {
	Context *tmp;
	tmp = malloc(sizeof(Context));
	memset(tmp,0,sizeof(Context));

	_copyColor(tmp->stroke,src->stroke);
	_copyColor(tmp->fill,src->fill);
	_copyColor(tmp->tint,src->tint);
	tmp->strokeWeight = src->strokeWeight;
	tmp->strokeCap = src->strokeCap;
	tmp->strokeJoin = src->strokeJoin;
	tmp->imageMode = src->imageMode;
	tmp->rectMode = src->rectMode;
	tmp->ellipseMode = src->ellipseMode;
	tmp->shapeMode = src->shapeMode;
	tmp->textAlign = src->textAlign;
	tmp->textFont = src->textFont;
	tmp->textSize = src->textSize;
	tmp->textLeading = src->textLeading;
	tmp->next = src->next;
	return tmp;
}

VGImage images[100];
int iWidth[100];
int iHeight[100];
VGFont fonts[100];
int fHeight[100];
int fSize[100];
VGPath paths[100];
int pathSize = 0;

int imageCount = 0;
int fontCount = 1;

int pathId=0;
int imageId=0;

bool done=false;
bool loop=true;
bool noEvent=true;
int fontId=0;
int frameRate=60;
bool noStroke=false;
bool noFill=true;
int frameCount = 0;
int _frameCount = 0;
int textSize = 0;
int alignX = P5_LEFT;
int alignY = P5_BASELINE;
int mouseButton;
char keyPress[2] = "X\0";
int eventType = -1;
int keyCode = 0;
bool initialized = false;

VGint width=640, height=480;
char mainPath[256];
char scriptname[256];

lua_State *L;
VGubyte seg;

char ttf_buffer[1<<24];

void bail(lua_State *L, int error, char *msg){
//	fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",msg, lua_tostring(L, -1));
	printf("\nFATAL ERROR:\n  %s: %s\n\n",msg, lua_tostring(L, -1));
	exit(1);
}

static void vg_call(char *fn_name);

static int resizeWindow(int,int);

// Structure commands:
// exit()
// loop()
// noLoop()
// popStyle()
// pushStyle()
// redraw()

static int P5_Time(lua_State *L) {
    struct timeval tv;
    long long t;

    gettimeofday(&tv,NULL);
    t = (tv.tv_sec*1000)+(tv.tv_usec/1000);
    lua_pushnumber(L,t);
    return 1;
}

static int P5_Exit(lua_State *L) {
	done = false;
}

static int P5_Loop(lua_State *L) {
	loop = true;
	return 0;
}

static int P5_NoLoop(lua_State *L) {
	loop = false;
	return 0;
}

static int P5_PopStyle(lua_State *L) {
	Context* temp = ctx;
	ctx = ctx->next;
	_SetFill(ctx->fill);
	_SetStroke(ctx->stroke);
	_SetStrokeWeight(ctx->strokeWeight);
	_SetStrokeCap(ctx->strokeCap);
	_SetStrokeJoin(ctx->strokeJoin);
	free(temp);
	return 0;
}

static int P5_PushStyle(lua_State *L) {
	Context* temp;
	temp = ctx_clone(ctx);
	temp->next = ctx;
	ctx = temp;
	return 0;
}

static int P5_Redraw(lua_State *L) {
	vg_call("draw");
	return 0;
}

// Environment commands:
// frameCount()
// frameRate()
// fullScreen()
// height()
// noSmooth()
// size()
// smooth()
// width()

static int P5_FrameCount(lua_State *L) {
	lua_pushnumber(L,frameCount);
	return 1;
}

static int P5_FrameRate(lua_State *L) {  
	frameRate = luaL_checknumber(L, 1);
	return 0;
}

static int P5_FullScreen(lua_State *L) {  
	return 0;
}

static int P5_Height(lua_State *L) {  
	lua_pushnumber(L, height);
	return 1;
}

static int P5_NoSmooth(lua_State *L) {
	glDisable(GL_MULTISAMPLE);
	return 0;
}

static int P5_Size(lua_State *L) {
	width = (int)luaL_checknumber(L, 1);
	height = (int)luaL_checknumber(L, 2);
	resizeWindow(width,height);
	vgResizeSurfaceSH(width,height);
	return 0;
}

static int P5_Smooth(lua_State *L) {
	glEnable(GL_MULTISAMPLE);
	return 0;
}

static int P5_Width(lua_State *L) {
	lua_pushnumber(L, width);
	return 1;
}

// 2D Primitives commands:
// arc()
// ellipse()
// line()
// point()
// quad()
// rect()
// triangle()

static int P5_Arc(lua_State *L) {
	VGUArcType arcType = VGU_ARC_PIE;
	VGfloat x,y,a,b,start,stop,mode;
	x = luaL_checknumber(L, 1);
	y = luaL_checknumber(L, 2);
	a = luaL_checknumber(L, 3);
	b = luaL_checknumber(L, 4);
	start = _deg(luaL_checknumber(L, 5));
	stop = _deg(luaL_checknumber(L, 6));
	if (lua_gettop(L) == 7) 
		mode = luaL_checkint(L, 7);
	else
		mode = 0;
	if (mode == P5_PIE)
		arcType = VGU_ARC_PIE;
	else if (mode == P5_CHORD)
		arcType = VGU_ARC_CHORD;
	else if (mode == P5_OPEN)
		arcType = VGU_ARC_OPEN;
	vgClearPath(shape_path,VG_PATH_CAPABILITY_APPEND_TO);
	switch (ctx->ellipseMode) {
	case P5_CORNERS:
		vguArc(shape_path,(x+a)/2,(y+b)/2, a-x, b-y,start,stop-start,arcType);
		break;
	case P5_CENTER:
		vguArc(shape_path,x,y,a,b,start,stop-start,arcType);
		break;
	case P5_RADIUS:
		vguArc(shape_path,x,y,a*2,b*2,start,stop-start,arcType);
	   	break;
	case P5_CORNER:
		vguArc(shape_path,x+a/2,y+b/2,a,b,start,stop-start,arcType);
	   	break;
	}
	_FillPath(shape_path);
	_StrokePath(shape_path);
	_EventPath(shape_path);
}

static int P5_Ellipse(lua_State *L) {
	VGfloat x,y,w,h;
	x = luaL_checknumber(L, 1);
	y = luaL_checknumber(L, 2);
	w = luaL_checknumber(L, 3);
	h = luaL_checknumber(L, 4);
	vgClearPath(shape_path,VG_PATH_CAPABILITY_APPEND_TO);
	switch (ctx->ellipseMode) {
	case P5_CORNERS:
		vguEllipse(shape_path,(x+w)/2,(y+h)/2, abs(w-x), abs(h-y));
		break;
	case P5_CENTER:
		vguEllipse(shape_path,x,y,abs(w),abs(h));
		break;
	case P5_RADIUS:
		vguEllipse(shape_path,x,y,abs(w*2),abs(h*2));
		break;
	case P5_CORNER:
		vguEllipse(shape_path,x+w/2,y+h/2,abs(w),abs(h));
		break;
	}
	_FillPath(shape_path);
	_StrokePath(shape_path);
	_EventPath(shape_path);
}

static int P5_Line(lua_State *L) {
	const VGfloat coords[4] = {
		luaL_checknumber(L, 1),luaL_checknumber(L, 2),
		luaL_checknumber(L, 3),luaL_checknumber(L, 4)
	};
	vgModifyPathCoords(line_path,0,2,coords);
	_StrokePath(line_path);
	_EventPath(line_path);
}

static int P5_Point(lua_State *L) {
	VGfloat x,y;
	vgClearPath(shape_path,VG_PATH_CAPABILITY_APPEND_TO);
	x = luaL_checknumber(L, 1); 
	y = luaL_checknumber(L, 2);
	vguEllipse(shape_path,x-2,y-2,4,4);
	_FillPath(shape_path);
	_StrokePath(shape_path);
	_EventPath(shape_path);
}

static int P5_Quad(lua_State *L) {  
	const VGfloat coords[8] = {
		luaL_checknumber(L, 1),luaL_checknumber(L, 2),
		luaL_checknumber(L, 3),luaL_checknumber(L, 4),
		luaL_checknumber(L, 5),luaL_checknumber(L, 6),
		luaL_checknumber(L, 7),luaL_checknumber(L, 8)
	}; 
	vgModifyPathCoords(quad_path, 0, 4, coords);
	_FillPath(quad_path);
	_StrokePath(quad_path);
	_EventPath(quad_path);
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
	vgClearPath(shape_path,VG_PATH_CAPABILITY_APPEND_TO);
	switch (ctx->rectMode) {
	case P5_CORNER:
		vguRoundRect(shape_path, x, y, a, b,r1,r2);
		break;
	case P5_CENTER:
		vguRoundRect(shape_path, x-a/2, y-b/2, a, b,r1,r2);
		break;
	case P5_RADIUS:
		vguRoundRect(shape_path, x-a/2, y-b/2, a/2, b/2,r1,r2);
		break;
	case P5_CORNERS:
		vguRoundRect(shape_path, x, y, x+a, y+b,r1,r2);
		break;
	}
	_FillPath(shape_path);
	_StrokePath(shape_path);
	_EventPath(shape_path);
}

static int P5_Rect(lua_State *L) {
	VGfloat x,y,a,b;
	if (lua_gettop(L)==5)
		return _RoundRect(L);
	x = luaL_checknumber(L, 1);
	y = luaL_checknumber(L, 2);
	a = luaL_checknumber(L, 3);
	b = luaL_checknumber(L, 4);
	VGfloat coords[5] = {x,y,a,b,-a};
	switch (ctx->rectMode) {
	case P5_CORNER:
		break;
	case P5_CENTER:
		coords[0] = x-a/2.0f; coords[1] = y-b/2.0f;
		break;
	case P5_RADIUS:
		coords[0] = x-a/2.0f; coords[1] = y-b/2.0f;
		coords[2] = a/2.0f; coords[3] = b/2.0f;
		coords[4] = -a/2.0f;
		break;
	case P5_CORNERS:
		coords[2] = a-x; coords[3] = b-y;
		coords[4] = -a+x;
		break;
	}
	vgModifyPathCoords(rect_path, 0, 4, coords);
	_FillPath(rect_path);
	_StrokePath(rect_path);
	_EventPath(rect_path);
}

static int P5_Triangle(lua_State *L) {
	const VGfloat coords[8] = {
		luaL_checknumber(L, 1),luaL_checknumber(L, 2),
		luaL_checknumber(L, 3),luaL_checknumber(L, 4),
		luaL_checknumber(L, 5),luaL_checknumber(L, 6)
	}; 
	vgModifyPathCoords(triangle_path, 0, 3, coords);
	_FillPath(triangle_path);
	_StrokePath(triangle_path);
	_EventPath(triangle_path);
}

// Curves commands:
// bezier()
// curve()

static int P5_Bezier(lua_State *L) {
	const VGfloat coords[8] = { 
		luaL_checknumber(L,1),luaL_checknumber(L,2),
		luaL_checknumber(L,3),luaL_checknumber(L,4),
		luaL_checknumber(L,5),luaL_checknumber(L,6),
		luaL_checknumber(L,7),luaL_checknumber(L,8)
	};
	vgModifyPathCoords(bezier_path, 0, 2, coords);
	_FillPath(bezier_path);
	_StrokePath(bezier_path);
	_EventPath(bezier_path);
}

static int P5_Curve(lua_State *L) {
	return 0;
}

// Attributes commands:
// ellipseMode()
// rectMode()
// strokeCap()
// strokeJoin()
// strokeWeight()

static int P5_EllipseMode(lua_State *L) {  
	ctx->ellipseMode = luaL_checkint(L, 1);
	return 0;
}

static int P5_RectMode(lua_State *L) {
	ctx->rectMode = luaL_checkint(L, 1);
	return 0;
}

static int P5_StrokeCap(lua_State *L) {
	_SetStrokeCap(luaL_checkint(L, 1));
	return 0;
}

static int P5_StrokeJoin(lua_State *L) {
	_SetStrokeJoin(luaL_checkint(L, 1));
	return 0;
}

static int P5_StrokeWeight(lua_State *L) {
	_SetStrokeWeight(luaL_checkint(L, 1));
	return 0;
}

// Vertex commands:
// beginShape()
// bezierVertex()
// curveVertex()
// endShape()
// quadraticVertex()
// vertex()

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

// Loading & Displaying commands:
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
	paths[pathId]=vgCreatePath(VG_PATH_FORMAT_STANDARD,VG_PATH_DATATYPE_F,1.0f,0.0f,0,0,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vgAppendPath(paths[pathId],shape_path);
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

	VGfloat matrix[9];
	vgSeti(VG_MATRIX_MODE,VG_MATRIX_PATH_USER_TO_SURFACE);
	vgGetMatrix(matrix);
	vgTranslate(x,y);
	vgScale(sx,sy);

	_FillPath(paths[pathId]);
	_StrokePath(paths[pathId]);
	
	vgLoadMatrix(matrix);
	_EventPath(paths[pathId]);
}

static int P5_ShapeMode(lua_State *L) {
	ctx->shapeMode = luaL_checkint(L, 1);
	return 0;
}

// Mouse commands:
// mouseButton()
// mouseClicked()
// mouseDragged()
// mouseMoved()
// mousePressed()
// mouseReleased()
// mouseWheel()
// mouseX()
// mouseY()
// pmouseX()
// pmouseY()


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

// Keyboard commands:
// key
// keyCode
// keyPressed()
// keyPressed
// keyReleased()
// keyTyped()

static int P5_IsKeyPressed(lua_State *L) {
	lua_pushboolean(L,isEvent[KEY_PRESSED]);
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

// Output Image commands:
// save()
// saveFrame()

static int P5_SaveFrame(lua_State *L) {
	return 0;
}

// Transform commands:
// popMatrix()
// printMatrix()
// pushMatrix()
// resetMatrix()
// rotate()
// scale()
// shearX()
// shearY()
// translate()

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

// Color Setting commands:
// background()
// colorMode()
// fill()
// noFill()
// noStroke()
// stroke()

static int _SetStyle(VGfloat* color,lua_State *L) {
	color[3] = 1;
	char* str; VGuint hexValue;
	if (lua_type(L,1) == LUA_TSTRING) {
		str = luaL_checkstring(L, 1);
		str[0]=' ';
		hexValue = strtol(str,NULL,16);
	} else
		hexValue = luaL_checknumber(L, 1);
	if (hexValue > 255) {
		color[0] = ((hexValue >> 16) & 0xFF)/255.0;
		color[1] = ((hexValue >> 8) & 0xFF)/255.0;
		color[2] = ((hexValue) & 0xFF)/255.0;
	} else {
		color[0] = hexValue/255.0;
		color[1] = color[0];
		color[2] = color[0];
	}
	if (lua_gettop(L) == 2)
		color[3] = luaL_checknumber(L, 2)/255.0;
	if (lua_gettop(L) > 2) {
		color[0] = luaL_checknumber(L, 1)/255.0;
		color[1] = luaL_checknumber(L, 2)/255.0;
		color[2] = luaL_checknumber(L, 3)/255.0;
		if (lua_gettop(L) == 4)
		  color[3] = luaL_checknumber(L, 4)/255.0;
	}
	return 0;
}

static int P5_Background(lua_State *L) {
	VGfloat color[4];
	_SetStyle(color,L);
	vgSetfv(VG_CLEAR_COLOR, 4, color);
	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgClear(0,0,width,height);
	return 0;
}

static int P5_ColorMode(lua_State *L) {
	ctx->colorMode = luaL_checkint(L, 1);
	return 0;
}

static int P5_Fill(lua_State *L) {
	noFill = false;
	VGfloat color[4];
	_SetStyle(color,L);
	_copyColor(ctx->fill,color);
	_SetFill(ctx->fill);
	return 0;
}

static int P5_NoFill(lua_State *L) {
	noFill = true;
	return 0;
}

static int P5_NoStroke(lua_State *L) {
	noStroke = true;
	return 0;
}

static int P5_Stroke(lua_State *L) {
	noStroke = false;
	VGfloat color[4];
	_SetStyle(color,L);
	_copyColor(ctx->stroke,color);
	_SetStroke(ctx->stroke);
	return 0;
}

// Color Creating & Reading commands:
// alpha()
// blue()
// brightness()
// color()
// green()
// hue()
// lerpColor()
// red()
// saturation()

static int P5_Alpha(lua_State *L) {   
   VGuint hexValue = luaL_checkint(L, 1);
   VGubyte a = ((hexValue) & 0xFF);
   lua_pushnumber(L,a);
	return 1;
}

static int P5_Blue(lua_State *L) {   
	VGuint hexValue = luaL_checkint(L, 1);
   VGubyte b = ((hexValue >> 8) & 0xFF);
   lua_pushnumber(L,b);
	return 1;
}

static int P5_Color(lua_State *L) {	
  int r,g,b,a;
  if (lua_gettop(L)==1) {
	r = g = b = luaL_checknumber(L, 1);
	a = 255; 
  } else if (lua_gettop(L)==2) {
	r = g = b = luaL_checknumber(L, 1);
	a = luaL_checknumber(L, 2);
  } else if (lua_gettop(L)==3) {
	r = luaL_checknumber(L, 1);
	g = luaL_checknumber(L, 2);
	b = luaL_checknumber(L, 3);
	a = 255;
  } else if (lua_gettop(L)==4) {
	r = luaL_checknumber(L, 1);
	g = luaL_checknumber(L, 2);
	b = luaL_checknumber(L, 3);
	a = luaL_checknumber(L, 4);
  }
  int c = ((a & 0xff) << 24) + ((r & 0xff) << 16) + 
		((g & 0xff) << 8) + (b & 0xff);
  lua_pushnumber(L,c);
  return 1;
}
		

static int P5_Green(lua_State *L) {   
   VGuint hexValue = luaL_checkint(L, 1);
   VGubyte g = ((hexValue >> 16) & 0xFF);
   lua_pushnumber(L,g);
   return 1;
}

static int P5_LerpColor(lua_State *L) {
	VGuint ca = luaL_checkint(L, 1);
	VGuint cb = luaL_checkint(L, 2);
	float t = luaL_checknumber(L, 3);
	
	VGubyte aa = ((ca >> 24) & 0xFF);
	VGubyte ar = ((ca >> 16) & 0xFF);
	VGubyte ag = ((ca >> 8) & 0xFF);
	VGubyte ab = ((ca) & 0xFF);
	
	VGubyte ba = ((cb >> 24) & 0xFF);
	VGubyte br = ((cb >> 16) & 0xFF);
	VGubyte bg = ((cb >> 8) & 0xFF);
	VGubyte bb = ((cb) & 0xFF);
	
	VGubyte r = ar + (br - ar) * t;
	VGubyte g = ag + (bg - ag) * t;
	VGubyte b = ab + (bb - ab) * t;
	VGubyte a = aa + (ba - aa) * t;

	int c = ((a & 0xff) << 24) + ((r & 0xff) << 16) + 
		((g & 0xff) << 8) + (b & 0xff);
   lua_pushnumber(L,c);
   return 1;
}

static int P5_Red(lua_State *L) {   
	VGuint hexValue = luaL_checkint(L, 1);
   VGubyte r = ((hexValue >> 24) & 0xFF);
   lua_pushnumber(L,r);
	return 1;
}

// Image commands:
// createImage()

static int P5_CreateImage(lua_State *L) {
	VGint width = luaL_checknumber(L, 1);  
	VGint height = luaL_checknumber(L, 2);
	imageCount++;
	unsigned int lilEndianTest = 1;
	VGImageFormat rgbaFormat;
	if (((unsigned char*)&lilEndianTest)[0] == 1)
		rgbaFormat = VG_sABGR_8888;
	else rgbaFormat = VG_sRGBA_8888;
	iWidth[imageCount] = width;
	iHeight[imageCount] = height;
	images[imageCount] = vgCreateImage(rgbaFormat,width,height,VG_IMAGE_QUALITY_BETTER);
	lua_pushnumber(L,imageCount);
	return 1;
}

// Image Loading & Displaying commands:
// imageWidth() -- Diököl extension
// imageHeight() -- Diököl extension
// image()
// imageMode()
// loadImage()
// noTint()
// requestImage()
// tint()

static int P5_ImageWidth(lua_State *L) {
   lua_pushnumber(L,iWidth[(int)luaL_checknumber(L, 1)]);
   return 1;
}

static int P5_ImageHeight(lua_State *L) {
   lua_pushnumber(L,iHeight[(int)luaL_checknumber(L, 1)]);
   return 1;
}

static int P5_Image(lua_State *L) {
	VGfloat matrix[9];
	VGPaint fill;
	int imageId = luaL_checknumber(L, 1);
	float w = iWidth[imageId];
	float h = iHeight[imageId];
	float x = luaL_checknumber(L,2);
	float y = luaL_checknumber(L,3);
	if (lua_gettop(L) == 5) {
		w = luaL_checknumber(L,4);
		h = luaL_checknumber(L,5);
	}
	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgGetMatrix(matrix);
	switch (ctx->imageMode) {
	  case P5_CENTER:
		x = x - w/2;
		y = y - h/2;
		break;
	  case P5_CORNERS:
		w = abs(w - x);
		h = abs(h - y);
		break;
	}
	float sx = w/iWidth[imageId];
	float sy = h/iHeight[imageId];
	vgTranslate(x,y);
	vgScale(sx,sy);
	vgDrawImage(images[imageId]);
	vgLoadMatrix(matrix);
	return 0;
}

static int P5_ImageMode(lua_State *L) {  
	ctx->imageMode = luaL_checkint(L, 1);
	return 0;
}

VGImage _createImageFromMemory(unsigned char const *buffer, int len , int* w, int* h) {
  VGImage img;
  VGint width;
  VGint height;
  VGint dstride;
  VGint n;
  VGubyte *data;
  int i,j;
  
  unsigned int lilEndianTest = 1;
  VGImageFormat rgbaFormat;
  if (((unsigned char*)&lilEndianTest)[0] == 1)
    rgbaFormat = VG_sABGR_8888;
  else rgbaFormat = VG_sRGBA_8888;
  
  data = stbi_load_from_memory(buffer, len, &width, &height, &n, STBI_rgb_alpha);
  dstride = width * 4;
  
  if (data == NULL) {
    printf("Failed creating image!\n");
    return VG_INVALID_HANDLE; 
  }
  
  *w = width;
  *h = height;
  img = vgCreateImage(rgbaFormat, width, height, VG_IMAGE_QUALITY_BETTER);
  vgImageSubData(img, data, dstride, rgbaFormat, 0, 0, width, height);
  stbi_image_free(data);
  return img;
}

VGImage _createImageFromFile(const char *filename, int* w, int* h) {
  VGImage img;
  VGint width;
  VGint height;
  VGint dstride;
  VGint n;
  VGubyte *data;
  int i,j;
  
  unsigned int lilEndianTest = 1;
  VGImageFormat rgbaFormat;
  if (((unsigned char*)&lilEndianTest)[0] == 1)
    rgbaFormat = VG_sABGR_8888;
  else rgbaFormat = VG_sRGBA_8888;
  
  char filepath[256];
  strcpy(filepath,mainPath);
    
  data = stbi_load(strcat(filepath,filename), &width, &height, &n, STBI_rgb_alpha);
  dstride = width * 4;
  
  if (data == NULL) {
    printf("Failed opening '%s' for reading!\n", filepath);
    return VG_INVALID_HANDLE; 
  }
  
  *w = width;
  *h = height;
  img = vgCreateImage(rgbaFormat, width, height, VG_IMAGE_QUALITY_BETTER);
  vgImageSubData(img, data, dstride, rgbaFormat, 0, 0, width, height);
  stbi_image_free(data);
  return img;
}

static int P5_ImportImage(lua_State *L) {
	const char *buffer = luaL_checkstring(L, 1);
	int len = luaL_checknumber(L, 2);
	imageCount++;
	images[imageCount]=_createImageFromMemory(buffer,len,&iWidth[imageCount],
		&iHeight[imageCount]);
	lua_pushnumber(L,imageCount);
	return 1;
}

static int P5_LoadImage(lua_State *L) {
	char filename[256];
	strcpy(filename,luaL_checkstring(L, 1));
	imageCount++;
	images[imageCount]=_createImageFromFile(filename,&iWidth[imageCount],
		&iHeight[imageCount]);
	lua_pushnumber(L,imageCount);
	return 1;
}

// Image Pixels commands:
// blend()
// copy()
// filter()
// get()
// loadPixels()
// pixels[]
// set()
// updatePixels()
// pixelsLength() -- Diököl extension

static int array_index (lua_State* L) {
   VGubyte** parray = luaL_checkudata(L, 1, "array");
   int index = luaL_checkint(L, 2);
   VGubyte r = (*parray)[index*4];
   VGubyte g = (*parray)[index*4+1];
   VGubyte b = (*parray)[index*4+2];
   VGubyte a = (*parray)[index*4+3];
   VGuint c = ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + 
		((b & 0xFF) << 8) + (a & 0xFF);
   lua_pushnumber(L,c);
   return 1;
}

static int array_newindex (lua_State* L) { 
   VGubyte** parray = luaL_checkudata(L, 1, "array");
   int index = luaL_checkint(L, 2);
   int hexValue = luaL_checkint(L, 3);
   VGubyte r = ((hexValue >> 24) & 0xFF);
   VGubyte g = ((hexValue >> 16) & 0xFF);
   VGubyte b = ((hexValue >> 8) & 0xFF);
   VGubyte a = ((hexValue) & 0xFF);
   (*parray)[index*4] = r;
   (*parray)[index*4+1] = g;
   (*parray)[index*4+2] = b;
   (*parray)[index*4+3] = a;
   return 0;
}

static int P5_PixelsLength (lua_State* L) { 
   VGubyte** parray = luaL_checkudata(L, 1, "array");
   VGubyte r = (*parray)[0];
   VGubyte g = (*parray)[1];
   VGubyte b = (*parray)[2];
   VGubyte a = (*parray)[3];
   VGuint c = ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + 
		((b & 0xFF) << 8) + (a & 0xFF);
   lua_pushnumber(L, c);
   return 1;
}

static int P5_LoadPixels(lua_State *L) {
	int imageId = luaL_checknumber(L, 1);
	VGImage image = images[imageId];
	int width = iWidth[imageId], height = iHeight[imageId];
	VGuint n = width*height;
	VGubyte** parray = lua_newuserdata(L,sizeof(VGubyte**));
	*parray = malloc(sizeof(VGubyte)*n*4+4);

	unsigned int lilEndianTest = 1;
	VGImageFormat rgbaFormat;
	if (((unsigned char*)&lilEndianTest)[0] == 1)
	  rgbaFormat = VG_sABGR_8888;
	else rgbaFormat = VG_sRGBA_8888;
   vgGetImageSubData(image, *parray+4,width*4,rgbaFormat,0,0,width,height);
   (*parray)[0] = ((n >> 24) & 0xFF);
   (*parray)[1] = ((n >> 16) & 0xFF);
   (*parray)[2] = ((n >> 8) & 0xFF);
   (*parray)[3] = ((n) & 0xFF);
   luaL_getmetatable(L, "array"); // 1024
   lua_setmetatable(L, -2);
   return 1;
}

static int P5_UpdatePixels(lua_State *L) {
	int imageId = luaL_checknumber(L, 1);
	VGubyte** parray = luaL_checkudata(L, 2, "array");
	VGImage image = images[imageId];
	int width = iWidth[imageId], height = iHeight[imageId];
	
	unsigned int lilEndianTest = 1;
	VGImageFormat rgbaFormat;
	if (((unsigned char*)&lilEndianTest)[0] == 1)
	  rgbaFormat = VG_sABGR_8888;
	else rgbaFormat = VG_sRGBA_8888;
	vgImageSubData(image,*parray+4,width*4,rgbaFormat,0,0,width,height);
	return 0;
}

// Rendering commands:
// clip()
// noClip()

static int P5_Clip(lua_State *L) {
	vgSetiv(VG_SCISSORING, VG_TRUE, 0);
	VGint coords[4] = { 
		luaL_checknumber(L, 1), luaL_checknumber(L, 2),
		luaL_checknumber(L, 3), luaL_checknumber(L, 4),
	};
	vgSetiv(VG_SCISSOR_RECTS, 4, coords);
	return 0;
}

static int P5_NoClip(lua_State *L) {
	vgSetiv(VG_SCISSORING, VG_FALSE, 0);
	return 0;
}

// Typography Loading & Displaying commands:
// createFont()
// loadFont()
// text()
// textFont()

VGFont _createFontFromFile(const char *filename, unsigned short size) {

	stbtt_fontinfo font;
	stbtt_vertex* vertices;
	float xpos=2;
	int i,advance,lsb,w,h,xoff=0,yoff=0;
	int x0,y0,x1,y1;
	int glyphIndex;
	int num_verts;
    
    char filepath[256];
    strcpy(filepath,mainPath);
    fread(ttf_buffer, 1, 1<<24, fopen(strcat(filepath,filename), "rb"));
   
	VGPath path;
	VGubyte seg;
	stbtt_InitFont(&font,ttf_buffer,stbtt_GetFontOffsetForIndex(ttf_buffer,0));
	stbtt_GetFontBoundingBox(&font,&x0,&y0,&x1,&y1);
  
	float scale = 10.0/(y1-y0);
   
	VGFont fnt = vgCreateFont(font.numGlyphs);
	for (glyphIndex=0;glyphIndex<font.numGlyphs;glyphIndex++) {
		stbtt_GetCodepointHMetrics(&font,(unsigned char)glyphIndex,&advance,&lsb);
		num_verts = stbtt_GetCodepointShape(&font,(unsigned char)glyphIndex,&vertices);
		path = vgCreatePath(VG_PATH_FORMAT_STANDARD,VG_PATH_DATATYPE_F,1.0f,0.0f,num_verts,4,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
		for (i=0; i < num_verts; ++i) {
         switch (vertices[i].type) {
            case STBTT_vmove:
               seg = VG_MOVE_TO;
               vgAppendPathData(path,1,&seg,
							(VGfloat[]){vertices[i].x*scale,vertices[i].y*scale});
               break;
            case STBTT_vline:
               seg = VG_LINE_TO;
               vgAppendPathData(path,1,&seg,
							(VGfloat[]){vertices[i].x*scale,vertices[i].y*scale});
               break;
            case STBTT_vcurve:
					seg = VG_QUAD_TO;
               vgAppendPathData(path,1,&seg,
							(VGfloat[]){vertices[i].cx*scale,vertices[i].cy*scale,
                               vertices[i].x*scale,vertices[i].y*scale});
               break;
            case STBTT_vcubic:
					seg = VG_CUBIC_TO;
					vgAppendPathData(path,1,&seg,
							(VGfloat[]){vertices[i].cx*scale,vertices[i].cy*scale,
                               vertices[i].cx1*scale,vertices[i].cy1*scale,
                               vertices[i].x*scale,vertices[i].y*scale});
               break;
         }
      }
      seg = VG_CLOSE_PATH;
      vgAppendPathData(path,1,&seg,(VGfloat[]){0.0});
		vgSetGlyphToPath(fnt,glyphIndex,path,VG_FALSE,(VGfloat[]){xoff*scale,yoff*scale},(VGfloat[]){advance*scale,0});
	}
   return fnt;
}

static int P5_CreateFont(lua_State *L) {
	char filename[256];
	strcpy(filename,luaL_checkstring(L, 1));
	int textHeight = luaL_checknumber(L, 2);
	fontCount++;
	fonts[fontCount]=_createFontFromFile(filename,textHeight);
	fHeight[fontCount] = fSize[fontCount] = textHeight;
	lua_pushnumber(L,fontCount);
	return 1;
}

VGFont _loadFontFromFile(const char *filename, unsigned short size) {

	stbtt_fontinfo font;
	VGubyte *bitmap;
	float scale, xpos=2;
	int i,advance,lsb,w,h,xoff,yoff;
	int x0,y0,x1,y1;
	int glyphIndex;

    char filepath[256];
    strcpy(filepath,mainPath);
    fread(ttf_buffer, 1, 1<<24, fopen(strcat(filepath,filename), "rb"));
   
    VGImage image;
	VGImageFormat bitmapFormat = VG_sL_8;
	unsigned int lilEndianTest = 1;
	VGImageFormat rgbaFormat;

	if (((unsigned char*)&lilEndianTest)[0] == 1)
		rgbaFormat = VG_sABGR_8888;
	else rgbaFormat = VG_sRGBA_8888;
	
   stbtt_InitFont(&font,ttf_buffer,stbtt_GetFontOffsetForIndex(ttf_buffer,0));
   VGFont fnt = vgCreateFont(font.numGlyphs);
   scale = stbtt_ScaleForPixelHeight(&font, size);
   for (glyphIndex=0;glyphIndex<font.numGlyphs;glyphIndex++) {
		stbtt_GetCodepointHMetrics(&font,(unsigned char)glyphIndex,&advance,&lsb);
		bitmap = stbtt_GetCodepointBitmap(&font,scale,scale,
								(unsigned char)glyphIndex, &w, &h, &xoff,&yoff);
		for (i=0; i< w*h; i++) bitmap[i]= 0xFF-bitmap[i];
		image = vgCreateImage(rgbaFormat,w,h,VG_IMAGE_QUALITY_NONANTIALIASED);
		vgImageSubData(image,bitmap,w,bitmapFormat,0,0,w,h);
		vgSetGlyphToImage(fnt,glyphIndex,image,(VGfloat[]){xoff,yoff},
											(VGfloat[]){(int)(advance*scale),0});
	}
   return fnt;
}

static int P5_LoadFont(lua_State *L) {
	char filename[256];
	strcpy(filename,luaL_checkstring(L, 1));
	int textHeight = luaL_checknumber(L, 2);
	fontCount++;
	fonts[fontCount]=_loadFontFromFile(filename,textHeight);
	fHeight[fontCount] = fSize[fontCount] = textHeight;
	lua_pushnumber(L,fontCount);
	return 1;
}

static int P5_Text(lua_State *L) {
	char str[256];
	strcpy(str,luaL_checkstring(L, 1));
	float x = luaL_checknumber(L, 2); 
	float y = luaL_checknumber(L, 3);
	VG_GLYPH_ORIGIN[0] = 0;
	VG_GLYPH_ORIGIN[1] = 0;
	int i;
	for (i=0;i<strlen(str);i++)
		vgDrawGlyph(fonts[fontId],str[i],0,false);
	int tWidth = VG_GLYPH_ORIGIN[0];
	switch (alignX) {
		case P5_LEFT:
		  break;
		case P5_RIGHT:
		  x = x - tWidth;
		  break;
		case P5_CENTER:
		  x = x - tWidth/2;
		  break;
	}
	VG_GLYPH_ORIGIN[0] = 0;
   VG_GLYPH_ORIGIN[1] = 0;
   VGfloat matrix[9];
	vgSeti(VG_MATRIX_MODE,VG_MATRIX_PATH_USER_TO_SURFACE);
	vgGetMatrix(matrix);
	vgTranslate(x,y);
	vgScale(fSize[fontId]/10.0f,-fSize[fontId]/10.0f);

   for (i=0;i<strlen(str);i++) {
		vgDrawGlyph(fonts[fontId],str[i],VG_FILL_PATH,false);
	}
//	for (i=0;i<strlen(str);i++)
//		vgDrawGlyph(fonts[fontId],str[i],VG_STROKE_PATH,false);
	vgLoadMatrix(matrix);
	return 0;
}

static int P5_TextFont(lua_State *L) {   
	fontId = luaL_checkint(L, 1);
	return 0;
}

// Typography Attributes commands
// textAlign()
// textLeading()
// textSize()
// textWidth()

static int P5_TextAlign(lua_State *L) {
	alignX = luaL_checkint(L, 1);
	if (lua_gettop(L) == 2)
		alignY = luaL_checkint(L, 2);
	return 0;
}

static int P5_TextLeading(lua_State *L) {
	ctx->textLeading = luaL_checkint(L, 1);
	return 0;
}

static int P5_TextSize(lua_State *L) {
    fHeight[fontId] = fSize[fontId] = luaL_checknumber(L, 1);
	return 0;
}

static int P5_TextWidth(lua_State *L) {
	int width = 0;
	char str[256];
	strcpy(str,luaL_checkstring(L, 1));
	VG_GLYPH_ORIGIN[0] = 0;
   VG_GLYPH_ORIGIN[1] = 0;
   int i;
   for (i=0;i<strlen(str);i++)
		vgDrawGlyph(fonts[fontId],str[i],0,false);
   lua_pushnumber(L,VG_GLYPH_ORIGIN[0]);
   return 1;
}

// Calculation functions:
// constrain()
// dist()
// lerp()
// mag()
// map()
// norm()

static int P5_Constrain(lua_State *L) {
	float amt,low,high,value;
	amt = luaL_checknumber(L, 1);
	low = luaL_checknumber(L, 2);
	high = luaL_checknumber(L, 3);
	if (amt < low)
		value = low;
	else if (amt > high)
		value = high;
	else
		value = amt;
	lua_pushnumber(L,value);
	return 1;
}

static int P5_Dist(lua_State *L) {
	float x1,y1,x2,y2;
	x1 = luaL_checknumber(L, 1);
	y1 = luaL_checknumber(L, 2);
	x2 = luaL_checknumber(L, 3); 
	y2 = luaL_checknumber(L, 4);
	#ifdef __linux__
	float d = STBTT_sqrt(STBTT_pow(x2-x1,2)+STBTT_pow(y2-y1,2));
	#else
	float d = sqrt(pow(x2-x1,2)+pow(y2-y1,2));
	#endif
	lua_pushnumber(L,d);
	return 1;
}

static int P5_Lerp(lua_State *L) {
	float a,b,f;
	a = luaL_checknumber(L, 1);
	b = luaL_checknumber(L, 2);
	f = luaL_checknumber(L, 3); 
	float l = a + f * (b - a);
	lua_pushnumber(L,l);
	return 1;
}

static int P5_Mag(lua_State *L) {
	float x,y;
	x = luaL_checknumber(L, 1);
	y = luaL_checknumber(L, 2);
	#ifdef __linux__
	float d = STBTT_sqrt(STBTT_pow(x,2)+STBTT_pow(y,2));
	#else
	float d = sqrt(pow(x,2)+pow(y,2));
	#endif
	lua_pushnumber(L,d);
	return 1;
}

static int P5_Map(lua_State *L) {  
	float value,start1,stop1,start2,stop2;
	value = luaL_checknumber(L, 1);
	start1 = luaL_checknumber(L, 2);
	stop1 = luaL_checknumber(L, 3); 
	start2 = luaL_checknumber(L, 4);
	stop2 = luaL_checknumber(L, 5);
	float outgoing =
		start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	lua_pushnumber(L, outgoing);
	return 1;
}

static int P5_Norm(lua_State *L) {
	float value,start1,stop1;
	value = luaL_checknumber(L, 1);
	start1 = luaL_checknumber(L, 2);
	stop1 = luaL_checknumber(L, 3); 
	float outgoing = ((value - start1) / (stop1 - start1));
	lua_pushnumber(L, outgoing);
	return 1;
}

// Trigonometry functions:
// degrees()
// radians()

static int P5_Degrees(lua_State *L) {
	float radians,degrees;
	radians = luaL_checknumber(L, 1);
	degrees = _deg(radians);
	lua_pushnumber(L, degrees);
	return 1;
}

static int P5_Radians(lua_State *L) {
	float radians,degrees;
	degrees = luaL_checknumber(L, 1);
	radians = _rad(degrees);
	lua_pushnumber(L, radians);
	return 1;
}

// Event functions:
// event()
// noEvent()
// getID()

static int P5_Event(lua_State *L) {
	int i; noEvent = false;
	evtn = lua_gettop(L);
	for (i=0;i<sizeof(event)/sizeof(event[0]);i++)
		event[i] = false;
	for (i=0;i<evtn;i++)
		event[luaL_checkinteger(L,i+1)] = true;
	return 0;
}

static int P5_NoEvent(lua_State *L) {
	noEvent = true;
	return 0;
}

static int P5_GenID(lua_State *L) {
	lua_Debug ar;
	lua_getstack(L, 1, &ar);
	lua_getinfo(L, "nSl", &ar);
	int line = ar.currentline;
	lua_pushnumber(L,line);
   return 1;
}

// OpenVG functions

void vg_init(int w,int h) {
	width = w; height = h;
	strokePaint = vgCreatePaint();
	vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

	fillPaint = vgCreatePaint();
	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

	ctx = ctx_new();
	_SetFill(ctx->fill);
	_SetStroke(ctx->stroke);
	_SetStrokeWeight(ctx->strokeWeight);
	_SetStrokeCap(ctx->strokeCap);
	_SetStrokeJoin(ctx->strokeJoin);

	temp_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,1.0f,0.0f,0,0, VG_PATH_CAPABILITY_APPEND_FROM | VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);

	shape_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,1.0f,0.0f,0,0, VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_APPEND_FROM | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);

	line_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,1.0f,0.0f,2,4, VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vguLine(line_path, 0.0f, 0.0f, 1.0f, 1.0f);

	rect_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,1.0f,0.0f,5,5, VG_PATH_CAPABILITY_ALL);
	vguRect(rect_path, 0.0f, 0.0f, 1.0f, 1.0f);

	roundrect_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 10, 26,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vguRoundRect(roundrect_path, 0.0f, 0.0f, 2.0f, 2.0f, 1.0f, 1.0f);

	point_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 4, 12, VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vguEllipse(point_path, 0.5f, 0.5f, 1.0f, 1.0f);

	ellipse_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 4, 12,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vguEllipse(ellipse_path, 0.0f, 0.0f, 1.0f, 1.0f);

	VGubyte segments[] = { VG_MOVE_TO_ABS, VG_CUBIC_TO };
	VGfloat coords[] = { 0.0f, 0.0f, 1.0f, -1.0f, 2.0f, 1.0f, 3.0f, 0.0f };
	
	bezier_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 2, 8,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vgAppendPathData(bezier_path, 2, segments, coords);

	quad_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 2, 8,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vguPolygon(quad_path, coords, 4, 1);

	triangle_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 2, 6,VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS);
	vguPolygon(triangle_path, coords, 3, 1);

	vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgClear(0,0,480,640);
	vgSeti(VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_FASTER);
    //fonts[fontId]=_createFontFromFile("default.ttf",13);
    //fHeight[fontId] = fSize[fontId] = 13;
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

	eventType = WINDOW_RESIZED;
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
	if (eventType!=-1) 
		Dkl_ProcessEvent(eventType);
	if (loop&&initialized) vg_call("draw");
	eventType = -1;
	_resetEvents();
}

// Lua functions

static void create_array_type(lua_State* L) {
   static const struct luaL_reg array[] = {
      { "__index",  array_index  },
      { "__newindex",  array_newindex  },
      NULL, NULL
   };
   luaL_newmetatable(L, "array");
   luaL_openlib(L, NULL, array, 0);
}

int luaRegisterAPI(int argc, const char * argv[]) {

	create_array_type(L);
    
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

	lua_pushcfunction(L,P5_Exit);
	lua_setglobal(L,"exit");
	
	lua_pushcfunction(L,P5_Time);
	lua_setglobal(L,"time");

	lua_pushcfunction(L,P5_Loop);
	lua_setglobal(L,"loop");

	lua_pushcfunction(L,P5_NoLoop);
	lua_setglobal(L,"noLoop");
	
	lua_pushcfunction(L,P5_PopStyle);
	lua_setglobal(L,"popStyle");
	
	lua_pushcfunction(L,P5_PushStyle);
	lua_setglobal(L,"pushStyle");
	  
	lua_pushcfunction(L,P5_Redraw);
	lua_setglobal(L,"redraw");
 
	lua_pushcfunction(L,P5_FrameCount);
	lua_setglobal(L,"frameCount");
	
	lua_pushcfunction(L,P5_FrameRate);
	lua_setglobal(L,"frameRate");
	
	lua_pushcfunction(L,P5_FullScreen);
	lua_setglobal(L,"fullScreen");

	lua_pushcfunction(L,P5_Height);
	lua_setglobal(L,"height");
	
	lua_pushcfunction(L,P5_NoSmooth);
	lua_setglobal(L,"noSmooth");
	  
	lua_pushcfunction(L,P5_Size);
	lua_setglobal(L,"size");

	lua_pushcfunction(L,P5_Smooth);
	lua_setglobal(L,"smooth");
   
    lua_pushcfunction(L,P5_Width);
	lua_setglobal(L,"width");
	
	lua_pushcfunction(L,P5_Arc);
	lua_setglobal(L,"arc");
   
    lua_pushcfunction(L,P5_Ellipse);
	lua_setglobal(L,"ellipse");
	
	lua_pushcfunction(L,P5_Line);
	lua_setglobal(L,"line");
 
	lua_pushcfunction(L,P5_Point);
	lua_setglobal(L,"point");
	   
	lua_pushcfunction(L,P5_Quad);
	lua_setglobal(L,"quad");
	
	lua_pushcfunction(L,P5_Rect);
	lua_setglobal(L,"rect");
   
	lua_pushcfunction(L,P5_Triangle);
	lua_setglobal(L,"triangle");
	
	lua_pushcfunction(L,P5_Bezier);
	lua_setglobal(L,"bezier");
	
	lua_pushcfunction(L,P5_Curve);
	lua_setglobal(L,"curve");
 
	lua_pushcfunction(L,P5_EllipseMode);
	lua_setglobal(L,"ellipseMode");
	
	lua_pushcfunction(L,P5_RectMode);
	lua_setglobal(L,"rectMode");
   
   lua_pushcfunction(L,P5_StrokeCap);
	lua_setglobal(L,"strokeCap");
 
	lua_pushcfunction(L,P5_StrokeJoin);
	lua_setglobal(L,"strokeJoin");

	lua_pushcfunction(L,P5_StrokeWeight);
	lua_setglobal(L,"strokeWeight");
		
	lua_pushcfunction(L,P5_BeginShape);
	lua_setglobal(L,"beginShape");
   
  	lua_pushcfunction(L,P5_BezierVertex);
	lua_setglobal(L,"bezierVertex");
	
	lua_pushcfunction(L,P5_CurveVertex);
	lua_setglobal(L,"curveVertex");
	
	lua_pushcfunction(L,P5_EndShape);
	lua_setglobal(L,"endShape");
	
	lua_pushcfunction(L,P5_QuadraticVertex);
	lua_setglobal(L,"quadraticVertex");
	
	lua_pushcfunction(L,P5_Vertex);
	lua_setglobal(L,"vertex");
	
	lua_pushcfunction(L,P5_SaveShape);
	lua_setglobal(L,"saveShape");

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

	lua_pushcfunction(L,P5_Shape);
	lua_setglobal(L,"shape");
	
	lua_pushcfunction(L,P5_ShapeMode);
	lua_setglobal(L,"shapeMode");
	
	lua_pushcfunction(L,P5_SaveFrame);
	lua_setglobal(L,"saveFrame");
	
	lua_pushcfunction(L,P5_PopMatrix);
	lua_setglobal(L,"popMatrix");
	
	lua_pushcfunction(L,P5_PrintMatrix);
	lua_setglobal(L,"printMatrix");
  
	lua_pushcfunction(L,P5_PushMatrix);
	lua_setglobal(L,"pushMatrix");
	
	lua_pushcfunction(L,P5_ResetMatrix);
	lua_setglobal(L,"resetMatrix");
	
	lua_pushcfunction(L,P5_Rotate);
	lua_setglobal(L,"rotate");
	
	lua_pushcfunction(L,P5_Scale);
	lua_setglobal(L,"scale");
	
	lua_pushcfunction(L,P5_ShearX);
	lua_setglobal(L,"shearX");
	
	lua_pushcfunction(L,P5_ShearY);
	lua_setglobal(L,"shearY");
	
	lua_pushcfunction(L,P5_Translate);
	lua_setglobal(L,"translate");
	
	lua_pushcfunction(L,P5_Background);
	lua_setglobal(L,"background");

	lua_pushcfunction(L,P5_ColorMode);
	lua_setglobal(L,"colorMode");

	lua_pushcfunction(L,P5_Fill);
	lua_setglobal(L,"fill");

	lua_pushcfunction(L,P5_NoFill);
	lua_setglobal(L,"noFill");

	lua_pushcfunction(L,P5_NoStroke);
	lua_setglobal(L,"noStroke");

	lua_pushcfunction(L,P5_Stroke);
	lua_setglobal(L,"stroke");
  
	lua_pushcfunction(L,P5_Alpha);
	lua_setglobal(L,"alpha");
	
	lua_pushcfunction(L,P5_Blue);
	lua_setglobal(L,"blue");

	lua_pushcfunction(L,P5_Color);
	lua_setglobal(L,"color");
   
	lua_pushcfunction(L,P5_Green);
	lua_setglobal(L,"green");
  
	lua_pushcfunction(L,P5_LerpColor);
	lua_setglobal(L,"lerpColor");

	lua_pushcfunction(L,P5_Red);
	lua_setglobal(L,"red");
 
	lua_pushcfunction(L,P5_CreateImage);
	lua_setglobal(L,"createImage");

	lua_pushcfunction(L,P5_ImageWidth);
	lua_setglobal(L,"imageWidth");
 
	lua_pushcfunction(L,P5_ImageHeight);
	lua_setglobal(L,"imageHeight");

	lua_pushcfunction(L,P5_Image);
	lua_setglobal(L,"image");
	
	lua_pushcfunction(L,P5_ImageMode);
	lua_setglobal(L,"imageMode");
	
	lua_pushcfunction(L,P5_ImportImage);
	lua_setglobal(L,"importImage");
	
	lua_pushcfunction(L,P5_LoadImage);
	lua_setglobal(L,"loadImage");

	lua_pushcfunction(L,P5_LoadPixels);
	lua_setglobal(L,"loadPixels");
	
	lua_pushcfunction(L,P5_UpdatePixels);
	lua_setglobal(L,"updatePixels");

	lua_pushcfunction(L,P5_PixelsLength);
	lua_setglobal(L,"pixelsLength");

	lua_pushcfunction(L,P5_Clip);
	lua_setglobal(L,"clip");

	lua_pushcfunction(L,P5_NoClip);
	lua_setglobal(L,"noClip");

	lua_pushcfunction(L,P5_CreateFont);
	lua_setglobal(L,"createFont");

	lua_pushcfunction(L,P5_LoadFont);
	lua_setglobal(L,"loadFont");

	lua_pushcfunction(L,P5_Text);
	lua_setglobal(L,"text");

	lua_pushcfunction(L,P5_TextFont);
	lua_setglobal(L,"textFont");

	lua_pushcfunction(L,P5_TextAlign);
	lua_setglobal(L,"textAlign");

	lua_pushcfunction(L,P5_TextLeading);
	lua_setglobal(L,"textLeading");

	lua_pushcfunction(L,P5_TextSize);
	lua_setglobal(L,"textSize");

	lua_pushcfunction(L,P5_TextWidth);
	lua_setglobal(L,"textWidth");

	lua_pushcfunction(L,P5_Constrain);
	lua_setglobal(L,"constrain");

	lua_pushcfunction(L,P5_Dist);
	lua_setglobal(L,"dist");

	lua_pushcfunction(L,P5_Lerp);
	lua_setglobal(L,"lerp");

	lua_pushcfunction(L,P5_Mag);
	lua_setglobal(L,"mag");

	lua_pushcfunction(L,P5_Map);
	lua_setglobal(L,"map");

	lua_pushcfunction(L,P5_Norm);
	lua_setglobal(L,"norm");

	lua_pushcfunction(L,P5_Degrees);
	lua_setglobal(L,"degrees");

	lua_pushcfunction(L,P5_Radians);
	lua_setglobal(L,"radians");

	lua_pushcfunction(L,P5_Event);
	lua_setglobal(L,"event");

	lua_pushcfunction(L,P5_NoEvent);
	lua_setglobal(L,"noEvent");

	return 0;
}

void lua_init(int argc, const char * argv[]) {
	L = luaL_newstate();
	luaL_openlibs(L);
	luaRegisterAPI(argc,argv);
    
    char filepath[256];
    strcpy(filepath,mainPath);
	if (luaL_loadfile(L,scriptname))
		bail(L,0,"luaL_loadfile() failed");
	if ((error = lua_pcall(L, 0, 0, 0)))
		bail(L, error, "Error running script\n");
	vg_call("setup");
	initialized=true;
}

void lua_destroy() {
	lua_close(L);
}