#define GLFW_INCLUDE_NONE
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <P5Graphics/p5Graphics.h>

GLFWwindow *window;

void setup() {};

void draw() {};

void _size(int w, int h) { glfwSetWindowSize(window, w, h); }

void _setup() { setup(); }

void _draw() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  draw();
	glFinish();
	//glfwSwapBuffers(window);
  glfwPollEvents();
}

static void cursor_position_callback(GLFWwindow *window, double xpos,
                                     double ypos) {
  p5_motionEvent((int)xpos, (int)ypos);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  p5_keyPressedEvent(key, scancode);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	int bttn;
	int actn;

	switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
		bttn = P5_RIGHT;
		break;
		case GLFW_MOUSE_BUTTON_LEFT:
		bttn = P5_LEFT;
		break;
	}
	switch (action) {
		case GLFW_PRESS:
		actn = P5_PRESS;
		break;
		case GLFW_RELEASE:
		actn = P5_RELEASE;
		break;
	}
  p5_mouseEvent(bttn, actn);
}

int main() {

  if (!glfwInit())
    exit(-1);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_FALSE);

  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(-1);
  }

  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  
  glfwSwapInterval(1);
  glfwSetWindowSize(window, 640, 480);

  p5_setupFunc(&_setup);
  p5_drawFunc(&_draw);
  p5_sizeFunc(&_size);

  p5_init(640, 480);
  p5_loop();
  p5_mainLoop();

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(-1);
}