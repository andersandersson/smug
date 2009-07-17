#include "GL/glfw.h"
#include "graphics.h"

Rectangle* gVisibleArea = NULL;

Color Color_(float r, float g, float b, float a)
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
	
	gVisibleArea = Rectangle_new(0, 0, width, height);
	Graphics_setVisibleArea(gVisibleArea);
}

void Graphics_refresh()
{
	glfwSwapBuffers();
}

void Graphics_terminate()
{

}

void Graphics_setVisibleArea(Rectangle* rect)
{
	gVisibleArea->x = rect->x;
	gVisibleArea->y = rect->y;
	gVisibleArea->w = rect->w;
	gVisibleArea->h = rect->h;
	
  	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	glOrtho(gVisibleArea->x, gVisibleArea->x + gVisibleArea->w, gVisibleArea->y, gVisibleArea->y + gVisibleArea->h, -1, 1);
}

void Graphics_fillRect(Rectangle* rect, Color* color)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(color->r, color->g, color->b, color->a);
	glBegin(GL_POLYGON);
		glVertex3f(rect->x,           rect->y,           0.0f);
		glVertex3f(rect->x + rect->w, rect->y,           0.0f);
		glVertex3f(rect->x + rect->w, rect->y + rect->h, 0.0f);
		glVertex3f(rect->x,           rect->y + rect->h, 0.0f);
	glEnd();
	glPopAttrib();
}
