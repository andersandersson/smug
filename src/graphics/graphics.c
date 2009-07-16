#include "GL/glfw.h"
#include "graphics.h"

int Graphics_init(int width, int height, BOOL fullscreen)
{
  return glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 0, GLFW_WINDOW);
}

void Graphics_refresh()
{
	glfwSwapBuffers();
}

void Graphics_terminate()
{

}
