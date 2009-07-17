#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "engine/common.h"

typedef struct Color
{
	float r;
	float g;
	float b;
	float a;
} Color;

Color Color_create(float r, float g, float b, float a);

int Graphics_init(int width, int height, BOOL fullscreen);
void Graphics_refresh();
void Graphics_terminate();

#endif // GRAPHICS_H

