//#define UNICODE
//#define _UNICODE
#include <windows.h>
#include <stdio.h>
#include <gl/GL.h>
#include <vg/openvg.h>
#include <vg/vgu.h>
#include <vg/shFont.h>
#include "wglext.h"
#include "diokol.h"
#include "resource.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

// default window width
#if !defined(INITIAL_WINDOW_WIDTH)
#define INITIAL_WINDOW_WIDTH 640
#endif
// default window height
#if !defined(INITIAL_WINDOW_HEIGHT)
#define INITIAL_WINDOW_HEIGHT 480
#endif

HGLRC           hRC=FALSE; 
HDC             hDC=FALSE;
HWND            hWnd=FALSE;
HINSTANCE       hInstance=FALSE;

#define CLASS_NAME "Diokol"

int multisampleFormat = 0;
boolean multisampleSupported = FALSE;

HPALETTE hPalette = 0;            /* custom palette (if needed) */
MSG   msg;                /* message */
PIXELFORMATDESCRIPTOR pfd;

#define SWAPBUFFERS SwapBuffers(hDC)

int idTimer = -1;
int dragged = 0;
bool shiftPressed = false;
bool capsLockOn = false;

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static PAINTSTRUCT ps;
    RECT rect;
    char charPressed;
    int keyCode = 0;
    
    switch(uMsg) {

        case WM_CREATE:
            SetTimer(hWnd, idTimer = 1, 1000.0/frameRate, NULL);
            return 0;
            
        case WM_PAINT:
            drawScene();
            BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            return 0;
         
        case WM_SIZE:
            GetWindowRect(hWnd, &rect);
            vg_resize(rect.right-rect.left,rect.bottom-rect.top);
            PostMessage(hWnd, WM_PAINT, 0, 0);
            return 0;
            
        case WM_KEYDOWN:
            charPressed = wParam;
            if (wParam == VK_SHIFT) shiftPressed = true;
            if (wParam == VK_CAPITAL) capsLockOn = ! capsLockOn;
            if (wParam == VK_BACK) keyCode = P5_BACKSPACE;
            if (wParam == VK_TAB) keyCode = P5_TAB;
            if (wParam == VK_RETURN) keyCode = P5_RETURN;
            if (wParam == VK_ESCAPE) keyCode = P5_ESC;
            if (wParam == VK_DELETE) keyCode = P5_DELETE;
            if(isalpha(charPressed) && !shiftPressed && !capsLockOn)
                charPressed = tolower(charPressed);
            Dkl_KeyPressed(charPressed,keyCode);
            return 0;
            
        case WM_KEYUP:
            charPressed = wParam;
            if (wParam == VK_SHIFT) shiftPressed = false;
            if (wParam == VK_BACK) keyCode = P5_BACKSPACE;
            if (wParam == VK_TAB) keyCode = P5_TAB;
            if (wParam == VK_RETURN) keyCode = P5_RETURN;
            if (wParam == VK_ESCAPE) keyCode = P5_ESC;
            if (wParam == VK_DELETE) keyCode = P5_DELETE;
            if(isalpha(charPressed) && !shiftPressed && !capsLockOn)
                charPressed = tolower(charPressed);
            Dkl_KeyReleased(charPressed,keyCode);
            return 0;
            
        case WM_LBUTTONDOWN:
            Dkl_MousePressed(LOWORD(lParam), HIWORD(lParam),0);
            dragged = 1;
            return 0;
            
        case WM_RBUTTONDOWN:
            Dkl_MousePressed(LOWORD(lParam), HIWORD(lParam),1);
            return 0;
            
        case WM_LBUTTONUP:
            Dkl_MouseReleased(LOWORD(lParam), HIWORD(lParam),0);
            dragged = 0;
            return 0;
            
        case WM_RBUTTONUP:
            Dkl_MouseReleased(LOWORD(lParam), HIWORD(lParam),1);
            return 0;
            
        case WM_MOUSEMOVE:
            Dkl_MouseMoved(LOWORD(lParam), HIWORD(lParam),dragged);
            return 0;

        case WM_TIMER:
            drawScene();
            SWAPBUFFERS;
            return 0;

        case WM_CLOSE:
            done = TRUE;
            return 0;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

boolean wglExtensionSupported(const char *extension) {
    
    const size_t extlen = strlen(extension);
    const char *supported = NULL;
    const char *p;
    
    PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
    
    if (wglGetExtString)
        supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());
    if (supported == NULL)
        supported = (char*)glGetString(GL_EXTENSIONS);
    if (supported == NULL)
        return FALSE;
    for (p = supported; ; p++) {
        p = strstr(p, extension);
        if (p == NULL)
            return FALSE;
        if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
            return TRUE;
    }
}

boolean initMultisample(PIXELFORMATDESCRIPTOR pfd) {
    
    int pixelFormat;
    int valid;
    unsigned int numFormats;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    float fAttributes[] = {
        0.0f,
        0.0f
    };
    int iAttributes[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 24,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 16,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        WGL_SAMPLES_ARB, 8,  // Era 8
        0, 0
    };
    
    // see if the string exists in WGL
    if (!wglExtensionSupported("WGL_ARB_multisample")) {
        multisampleSupported = FALSE;
        return FALSE;
    }
    
    // get our pixel format
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    if (!wglChoosePixelFormatARB) {
        multisampleSupported = FALSE;
        return FALSE;
    }
    
    // first we check to see if we can get a pixel format for 8 samples
    valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
    if (valid && numFormats >= 1) {
        multisampleSupported = TRUE;
        multisampleFormat = pixelFormat;
        return TRUE;
    }
    
    // our pixel format with 8 samples failed, test for 6 samples
    iAttributes[19] = 6;
    valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
    if (valid && numFormats >= 1) {
        multisampleSupported = TRUE;
        multisampleFormat = pixelFormat;
        return TRUE;
    }
    
    // our pixel format with 6 samples failed, test for 4 samples
    iAttributes[19] = 4;
    valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
    if (valid && numFormats >= 1) {
        multisampleSupported = TRUE;
        multisampleFormat = pixelFormat;
        return TRUE;
    }
    
    // our pixel format with 4 samples failed, test for 2 samples
    iAttributes[19] = 2;
    valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
    if (valid && numFormats >= 1) {
        multisampleSupported = TRUE;
        multisampleFormat = pixelFormat;
        return TRUE;
    }
    
    multisampleSupported = FALSE;
    multisampleFormat = 0;
    return FALSE;
}

int CreateGLWindow(char* title, int width, int height) {

    RECT rect;
    WNDCLASS wc;
    int x, y, w, h;
	DWORD       dwExStyle;
	DWORD       dwStyle;
    GLuint pixelFormat;
    PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT = NULL;
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    
    static  PIXELFORMATDESCRIPTOR pfd= {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0, 0, 0, 0, 0, 0,
        0,
        8,
        0,
        0, 0, 0, 0,
        16,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
	
	if (!hInstance) {
        hInstance = GetModuleHandle(NULL);
    }
    wc.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = CLASS_NAME;
        
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "RegisterClass() failed:  "
                    "Cannot register window class.", "Error", MB_OK);
        return FALSE;
    }
    
    // calculate window size
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
    w = rect.right - rect.left;
    h = rect.bottom - rect.top;
    // center window
    x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
    
	if (!(hWnd=CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                CLASS_NAME,title,
                WS_TILEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                x, y,
                width,
                height,
                NULL, 
                NULL, 
                hInstance, 
                NULL))) {
		KillGLWindow();
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
    
    if (!(hDC=GetDC(hWnd))) {
        KillGLWindow();
        MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    
    if (!multisampleSupported) {
        if (!(pixelFormat = ChoosePixelFormat(hDC, &pfd))) {
            KillGLWindow();
            MessageBox(NULL, "Can't find a suitable pixel format.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
            return FALSE;
        }
    } else
        pixelFormat = multisampleFormat;
    
    if(!SetPixelFormat(hDC,pixelFormat,&pfd)) {
        KillGLWindow();
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    
    if (!(hRC=wglCreateContext(hDC))) {
        KillGLWindow();
        MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    
    if(!wglMakeCurrent(hDC,hRC)) {
        KillGLWindow();
        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    
    if (!multisampleSupported) {
        if (initMultisample(pfd)) {
            KillGLWindow();
            return CreateGLWindow(title, width, height);
        }
    }

    wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    if (wglGetExtensionsStringEXT != NULL && strstr(wglGetExtensionsStringEXT(), "WGL_EXT_swap_control")) {
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        // disable vsync
        if (wglSwapIntervalEXT != NULL)
            wglSwapIntervalEXT(0);
    }
    
    return TRUE;
}

int resizeWindow(int w,int h) {
    MoveWindow(hWnd,0,0,w,h,1);
}

int initOpenVG(int width, int height) {
    vgCreateContextSH(width,height);
    vg_init(width,height);
    lua_init();
    return TRUE;
}

void destroyOpenVG(void) {
    vgDestroyContextSH();
    lua_destroy();
}

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
        LPSTR lpszCmdLine, int nCmdShow) {
    
    LPWSTR *szArgList;
    int argCount = 0;

    szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
    if( argCount != 0 )
        strcpy(scriptname,szArgList[1]);
    else
        strcpy(scriptname,"main.lua");
    
    MessageBox(NULL,scriptname,"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
    
    hInstance = hCurrentInst;
    
    if (!CreateGLWindow("Diokol",INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT)) {
		return 0;
	}
    
    if (!initOpenVG(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT)) {
        KillGLWindow();
        return 0;
    }
    
    ShowWindow(hWnd, SW_NORMAL);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    
    while(GetMessage(&msg, hWnd, 0, 0)&&(!done)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    destroyOpenVG();
    KillGLWindow();
    return (msg.wParam);
}

GLvoid KillGLWindow(GLvoid) {
	if (hRC) {
	  if (!wglMakeCurrent(NULL,NULL)) {
		MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	  }
	  if (!wglDeleteContext(hRC)) {
		MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
      }
      hRC=NULL;
    }
    if (hDC && hWnd && !ReleaseDC(hWnd,hDC)) {
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;
	}
	if (hWnd && !DestroyWindow(hWnd)) {
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;
	}
	if (hInstance && !UnregisterClass(CLASS_NAME,hInstance)) {
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;
    }
}
