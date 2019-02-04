#import <Cocoa/Cocoa.h>

#include <OpenGL/gl.h>

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

id window;

@interface OpenGLView : NSOpenGLView <NSWindowDelegate> {
}
- (id) initWithFrame :(NSRect)frameRect;
- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
- (void) reshape;
- (void) dealloc;
- (unsigned int) initOpenVG :(int)width :(int)height;
- (void) destroyOpenVG;
// mouse and keyboard events
- (void) mouseMove :(NSEvent *)theEvent;
- (void) mouseDown :(NSEvent *)theEvent;
- (void) mouseUp :(NSEvent *)theEvent;
- (void) mouseDragged:(NSEvent *)theEvent;
- (void) rightMouseDown :(NSEvent *)theEvent;
- (void) rightMouseUp :(NSEvent *)theEvent;
- (void) rightMouseDragged:(NSEvent *)theEvent;
- (void) keyUp :(NSEvent *)theEvent;
- (void) keyDown :(NSEvent *)theEvent;
// responder properties
- (BOOL) acceptsFirstResponder;
- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;
- (BOOL) isFlipped;
@end

@implementation OpenGLView

-(void) windowWillClose:(NSNotification *)notification {
    [NSApp terminate:self];
}

- (void) dealloc {
    [self endOpenVG];
    [super dealloc];
    [window release];
}

// implementation of NSOpenGLView methods
- (id) initWithFrame :(NSRect)frameRect {
    
    NSOpenGLPixelFormatAttribute attributes [] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAMultisample,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAStencilSize, 3,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 3,
        (NSOpenGLPixelFormatAttribute)0
    };
    
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    
    if (!format) {
        NSLog(@"Unable to create pixel format.");
        exit(1);
    }
    
    self = [super initWithFrame: frameRect pixelFormat: format];
    if (self) {
        // the flushBuffer method executes as soon as possible
        [self setWantsBestResolutionOpenGLSurface:YES];
        GLint swapInt = 1;
        [[self openGLContext] makeCurrentContext];
        [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
        
        // get context and pixel format
        CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
        CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    }
    
    return self;
}

-(void) prepareOpenGL {
    if ([self lockFocusIfCanDraw]) {
        
        int surfaceWidth, surfaceHeight;
        // get frame dimensions
        NSSize bound = [self frame].size;
        int width = (int)bound.width;
        int height = (int)bound.height;
        
        // must lock GL context because display link is threaded
        [[self openGLContext] makeCurrentContext];
        CGLLockContext([[self openGLContext] CGLContextObj]);
        [[self openGLContext] update];
        
        GLint sync = 0;
        
        CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
        
        // Get view dimensions in pixels
        NSRect backingBounds = [self convertRectToBacking:[self bounds]];
        
        GLsizei backingPixelWidth  = (GLsizei)(backingBounds.size.width),
        backingPixelHeight = (GLsizei)(backingBounds.size.height);
        
        // Set viewport
        glViewport(0, 0, backingPixelWidth, backingPixelHeight);
        
        // init OpenVG
        if ([self initOpenVG :width :height]) {
            glEnable(GL_MULTISAMPLE);
        }
        else {
            NSLog(@"Unable to initialize Diokol.");
            exit(1);
        }
        
        // unlock the context
        CGLUnlockContext([[self openGLContext] CGLContextObj]);
        [self unlockFocus];
    }
}

- (void) reshape {
    
    // this method is called whenever the window/control is reshaped, it is also called when the control is first opened
    if ([self lockFocusIfCanDraw]) {
        
        int surfaceWidth, surfaceHeight;
        // get new dimensions
        NSSize bound = [self frame].size;
        int newWidth = (int)bound.width;
        int newWHeight = (int)bound.height;
        
        // must lock GL context because display link is threaded
        [[self openGLContext] makeCurrentContext];
        CGLLockContext([[self openGLContext] CGLContextObj]);
        [[self openGLContext] update];
        // resize OpenVG drawing surface
        
        vg_resize(newWidth,newWHeight);
        CGLUnlockContext([[self openGLContext] CGLContextObj]);
        [self unlockFocus];
    }
}

- (unsigned int) initOpenVG :(int)width :(int)height {
    vgCreateContextSH(width,height);
    vg_init(width,height);
    lua_init();
    return 1;
}

- (void) endOpenVG {
    vgDestroyContextSH();
    lua_destroy();
}

- (void) drawFrame {
    if ([self lockFocusIfCanDraw]) {
        [[self openGLContext] makeCurrentContext];
        CGLLockContext([[self openGLContext] CGLContextObj]);
        [[self openGLContext] update];
        drawScene();
        CGLUnlockContext([[self openGLContext] CGLContextObj]);
        [self unlockFocus];
        glFlush();
    }
}

- (void) drawRect :(NSRect)dirtyRect {
    
    (void)dirtyRect;
    if ([self lockFocusIfCanDraw]) {
        // must lock GL context because display link is threaded
        [[self openGLContext] makeCurrentContext];
        CGLLockContext([[self openGLContext] CGLContextObj]);
        
        // draw scene
        [self drawFrame];
        // flush OpenGL commands
        glFinish();
        [[self openGLContext] flushBuffer];
        // unlock the context
        CGLUnlockContext([[self openGLContext] CGLContextObj]);
        [self unlockFocus];
    }
}

// mouse and keyboard events
- (void) mouseDown: (NSEvent *)theEvent {
    NSPoint p;
    p = [theEvent locationInWindow];
    p = [self convertPoint: p fromView: nil];
    Dkl_MousePressed(p.x, p.y, 0);
}

- (void) mouseDragged:(NSEvent *)theEvent {
    NSPoint p;
    p = [theEvent locationInWindow];
    p = [self convertPoint: p fromView: nil];
    Dkl_MouseMoved(p.x, p.y, 1);
}

- (void) mouseUp: (NSEvent *)theEvent {
    NSPoint p;
    p = [theEvent locationInWindow];
    p = [self convertPoint: p fromView: nil];
    Dkl_MouseReleased(p.x, p.y, 0);
}

- (void) mouseMoved:(NSEvent *)theEvent {
    NSPoint p;
    p = [theEvent locationInWindow];
    p = [self convertPoint: p fromView: nil];
    Dkl_MouseMoved(p.x, p.y, 0);
}

- (void) rightMouseDown: (NSEvent *)theEvent {
    NSPoint p;
    p = [theEvent locationInWindow];
    p = [self convertPoint: p fromView: nil];
    Dkl_MousePressed(p.x, p.y, 1);
}

- (void) rightMouseDragged:(NSEvent *)theEvent {
    NSPoint p;
    p = [theEvent locationInWindow];
    p = [self convertPoint: p fromView: nil];
    Dkl_MouseMoved(p.x, p.y, 1);
}

- (void) rightMouseUp: (NSEvent *)theEvent {
    NSPoint p;
    p = [theEvent locationInWindow];
    p = [self convertPoint: p fromView: nil];
    Dkl_MouseReleased(p.x, p.y, 1);
}

- (void) keyDown:(NSEvent *)theEvent {
    char *str = [[theEvent characters] UTF8String];
    unsigned short keyCode = [theEvent keyCode];
    Dkl_KeyPressed(str[0],keyCode);
}

- (void) keyUp:(NSEvent *)theEvent {
    char *str = [[theEvent characters] UTF8String];
    unsigned short keyCode = [theEvent keyCode];
    Dkl_KeyReleased(str[0],keyCode);
}

- (BOOL) acceptsFirstResponder {
    return YES;
}

- (BOOL) becomeFirstResponder {
    return  YES;
}

- (BOOL) resignFirstResponder {
    return YES;
}

- (BOOL) isFlipped {
    return YES;
}

@end

static int resizeWindow(int w,int h) {
    NSRect frame;
    frame.size.height = h;
    frame.size.width = w;
    [window setFrame:frame display:YES animate:YES];
    [window center];
}

int main(int argc, const char * argv[]) {

  OpenGLView *view;
  NSRect frame = NSMakeRect(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    
  [NSAutoreleasePool new];
  [NSApplication sharedApplication];
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

  window = [[NSWindow alloc] initWithContentRect:frame styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask backing:NSBackingStoreBuffered defer: TRUE];

  //NSString *mPath = [NSBundle mainBundle].resourcePath;
  //strcat(strcpy(mainPath,[mPath UTF8String]),"/");
  
  if( argc >= 2 )
      strcpy(scriptname,argv[1]);
  else
      strcpy(scriptname,"main.lua");
        
  [window setAcceptsMouseMovedEvents:YES];
  [window setTitle: @"Diököl"];

  // create the OpenGL view
  view = [[OpenGLView alloc] initWithFrame: frame];
  [view setWantsBestResolutionOpenGLSurface:YES];

  // link the view to the window
  [window setDelegate: view];
  [window setContentView: view];
  [window makeFirstResponder: view];

  [view release];
  
  // center the window
  [window center];
  [window makeKeyAndOrderFront: nil];
  [NSApp activateIgnoringOtherApps:YES];

  dispatch_async(dispatch_get_main_queue(), ^{
    id m_timer = [NSTimer scheduledTimerWithTimeInterval:1.0/frameRate target:view
      selector:@selector(display) userInfo:nil repeats:YES];
  });

  [NSApp run];
  return 0;
}
