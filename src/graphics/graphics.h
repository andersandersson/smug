#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "engine/common.h"

int Graphics_init(int width, int height, BOOL fullscreen);
void Graphics_refresh();
void Graphics_terminate();

#endif // GRAPHICS_H

