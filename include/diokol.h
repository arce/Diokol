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

#include <VG/openvg.h>
#include <VG/vgu.h>
#include <VG/shFont.h>

#define P5_PI 3.14159265358979323846f
#define P5_TWO_PI 6.28318530718f
#define P5_HALF_PI 1.57079632679f
#define P5_QUARTER_PI 0.78539816339f

#ifndef M_PI
#    define M_PI 3.14159265358979323846f
#endif

#ifndef __APPLE__
typedef int bool;
#endif
#define true 1
#define false 0

#define PATH_SIZE 15

#define STROKE_JOIN 0
#define STROKE_CAP 1
#define STROKE_WEIGHT 2
#define STROKE_COLOR 3
#define FILL_COLOR 4

#define ELLIPSE 0
#define RECT 1
#define IMAGE 2
#define SHAPE 3
#define TEXT 4

int error;

VGPaint strokePaint;
VGPaint fillPaint;

VGPath paths[PATH_SIZE];

int mode[5] = {0,0,0,0,0}; // [0] = ellipse, [1] = rect, [2] = image, [3] = shape, [4] = text
int style[5] = {0,0,1,0xFF00FF00,0xFF000000}; // [0] = strokeJoin, [1] = strokeCap, [2] = strokeWeight, [3] = strokeColor, [4] = fillColor
int pathStyle[PATH_SIZE][5];
bool pathFree[PATH_SIZE];

bool done=false;
bool initialized=false;

VGint width=640, height=480;
char scriptname[256];
int frameRate = 60;

int clearColor = 0xAAAAAAFF;

lua_State *L;

void bail(lua_State *L, int error, char *msg){
  printf("\nFATAL ERROR:\n  %s: %s\n\n",msg, lua_tostring(L, -1));
  exit(1);
}

static void vg_call(char *fn_name);

static void app_close();

static int resizeWindow(int,int);

static void getArrColor(VGfloat* arr, int color) {
  arr[4] = ((color >> 24) & 0xFF)/255.0;
  arr[3] = ((color >> 16) & 0xFF)/255.0;
  arr[2] = ((color >> 8) & 0xFF)/255.0;
  arr[1] = ((color) & 0xFF)/255.0;
}

static int P5_Time(lua_State *L) {
  struct timeval tv;
  double t;
  gettimeofday(&tv,NULL);
  t = (tv.tv_sec*1000)+(tv.tv_usec/1000);
  lua_pushnumber(L,t);
  return 1;
}

static int P5_Exit(lua_State *L) {
  done = true;
  app_close();
}

static int P5_Size(lua_State *L) {
  width = (int)luaL_checknumber(L, 1);
  height = (int)luaL_checknumber(L, 2);
  resizeWindow(width,height);
  vgResizeSurfaceSH(width,height);
  return 0;
}

static void flushPath(int index) {
  VGfloat colour[4];
  if (pathStyle[index][FILL_COLOR]!=-1) {
	getArrColor(colour,pathStyle[index][FILL_COLOR]);
    vgSetParameterfv(fillPaint, VG_PAINT_COLOR, 4, colour);
    vgSetPaint(fillPaint, VG_FILL_PATH);
    vgDrawPath(paths[index], VG_FILL_PATH);
  }
  if (pathStyle[index][STROKE_COLOR]!=-1) {
	getArrColor(colour,pathStyle[index][STROKE_COLOR]);
    vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, colour);
    vgSetPaint(strokePaint, VG_STROKE_PATH);
    vgDrawPath(paths[index], VG_STROKE_PATH);
  }
  vgClearPath(paths[index], VG_PATH_CAPABILITY_APPEND_TO);
}

static int findPath(int style[5]) {
  int hash = 5381;

  for (int i=0; i<5; i++)
    hash = ((hash << 5) + hash) + style[i];

  int i = hash % PATH_SIZE;
  if (memcmp(style,pathStyle[i],sizeof(int)*5)==0) {
	memcpy(pathStyle[i],style,sizeof(int)*5);
	pathFree[i] = false;
	return i; 
  }
  int j = (i+1) % PATH_SIZE;
  while (j!=i) {
	if ((memcmp(style,pathStyle[j],sizeof(int)*5)==0) || pathFree[j]) {
	  memcpy(pathStyle[j],style,sizeof(int)*5);
	  pathFree[j] = false;
	  return j;
	}
	j = (j+1) % PATH_SIZE;
  }
  flushPath(i);
  memcpy(pathStyle[i],style,sizeof(int)*5);
  return i;
}

static int P5_Rect(lua_State *L) {

  VGfloat x,y,a,b;
  x = luaL_checknumber(L, 1);
  y = luaL_checknumber(L, 2);
  a = luaL_checknumber(L, 3);
  b = luaL_checknumber(L, 4);
  
  int index = findPath(style);
  vguRect(paths[index],x,y,a,b);
  if (vgGetParameteri(paths[index],VG_PATH_NUM_SEGMENTS)>=999)
	flushPath(index);
  return 0;
}

static void flushBuffers() {
  for (int i=0; i<PATH_SIZE; i++)
    flushPath(i);
}

static int P5_Flush(lua_State *L) {
  flushBuffers();
  return 0;
}

static int P5_StrokeCap(lua_State *L) {
  style[STROKE_CAP] = luaL_checkint(L, 1);
  return 0;
}

static int P5_StrokeJoin(lua_State *L) {
  style[STROKE_JOIN] = luaL_checkint(L, 1);
  return 0;
}

static int P5_StrokeWeight(lua_State *L) {
  style[STROKE_WEIGHT] = luaL_checkint(L, 1);
  return 0;
}

static int Color(lua_State *L) {
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
  return ((a & 0xff) << 24) + ((r & 0xff) << 16) + 
		((g & 0xff) << 8) + (b & 0xff);
}

static int P5_Background(lua_State *L) {
  VGfloat colour[4];
  getArrColor(colour,Color(L));
  vgSetfv(VG_CLEAR_COLOR, 4, colour);
  vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
  vgClear(0,0,width,height);
  return 0;
}

static int P5_Fill(lua_State *L) {
  style[FILL_COLOR] = Color(L);
  return 0;
}

static int P5_NoFill(lua_State *L) {
  style[FILL_COLOR] = -1;
  return 0;
}

static int P5_Stroke(lua_State *L) {
  style[STROKE_COLOR] = Color(L);
  return 0;
}

static int P5_NoStroke(lua_State *L) {
  style[STROKE_COLOR] = -1;
  return 0;
}

static int P5_Color(lua_State *L) {
  int c = Color(L);
  lua_pushnumber(L,c);
  return 1;
}

// OpenVG functions

void vg_init(int w,int h) {
	VGfloat colour[4];
	
	width = w; height = h;
	strokePaint = vgCreatePaint();
	vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);

	fillPaint = vgCreatePaint();
	vgSetParameteri(fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	
	int style[] = {0,0,1,-1,-1};
	
	for (int i=0; i<PATH_SIZE; i++) {
	  paths[i] = vgCreatePath(VG_PATH_FORMAT_STANDARD,VG_PATH_DATATYPE_F,1.0f,0.0f,1000,4,VG_PATH_CAPABILITY_APPEND_FROM);
	  memcpy(pathStyle[i],style,sizeof(int)*5);
	  pathFree[i] = true;
	}
    
    getArrColor(colour,clearColor);
	vgSetfv(VG_CLEAR_COLOR, 4, colour);
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

int drawScene() {
  if (initialized) { 
    vg_call("draw");
    flushBuffers();
  }
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
	
	lua_pushnumber(L,P5_PI);
	lua_setglobal(L,"PI");
	   
	lua_pushnumber(L,P5_TWO_PI);
	lua_setglobal(L,"TWO_PI"); 
	
	lua_pushnumber(L,P5_HALF_PI);
	lua_setglobal(L,"HALF_PI"); 
	
	lua_pushnumber(L,P5_QUARTER_PI);
	lua_setglobal(L,"QUARTER_PI"); 

// functions

    lua_pushcfunction(L,P5_Flush);
	lua_setglobal(L,"flush");
	
	lua_pushcfunction(L,P5_Exit);
	lua_setglobal(L,"exit");
	
	lua_pushcfunction(L,P5_Time);
	lua_setglobal(L,"time");
	  
	lua_pushcfunction(L,P5_Size);
	lua_setglobal(L,"size");
	
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

	lua_pushcfunction(L,P5_Fill);
	lua_setglobal(L,"fill");

	lua_pushcfunction(L,P5_NoFill);
	lua_setglobal(L,"noFill");

	lua_pushcfunction(L,P5_Stroke);
	lua_setglobal(L,"stroke");
	
	lua_pushcfunction(L,P5_NoStroke);
	lua_setglobal(L,"noStroke");

	lua_pushcfunction(L,P5_Color);
	lua_setglobal(L,"color");

	return 0;
}

void lua_init(int argc, const char * argv[]) {
	print("lua_init\n");
	L = luaL_newstate();
	luaL_openlibs(L);
	luaRegisterAPI(argc,argv);
    
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

void Dkl_KeyPressed() {}

void Dkl_KeyReleased() {}

void Dkl_MouseMoved() {}

void Dkl_MousePressed() {}

void Dkl_MouseReleased() {}
