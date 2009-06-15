#include "GL/glfw.h"
#include "graphics.h"

int Graphics_Init(int width, int height, char fullscreen)
{
  return glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 0, GLFW_WINDOW);
}
