#include "GL/glfw.h"
#include "graphics.h"
#include "../utils/rectangle.h"

Color Color_create(float r, float g, float b, float a)
{
	Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}

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

void Graphics_fillRect(Rectangle* rect, Color* color)
{
	
}