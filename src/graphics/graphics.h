/** @file graphics.h
  * @brief Contains functions to control graphics.
  */

#ifndef SMUG_GRAPHICS_GRAPHICS_H
#define SMUG_GRAPHICS_GRAPHICS_H

#include "color.h"
#include "common/common.h"
#include "utils/rectangle.h"
#include "drawable/drawable.h"

/**
 * Initialize Graphics subsystem
 *
 * This will create a window of a given size and set it
 * in either fullscreen or windowed mode.
 * @param width The width of the screen in pixels.
 * @param height The height of the screen in pixels.
 */
int Graphics_init(int width, int height);

/** Render one frame.
  */
void Graphics_render();

/** Registers a drawable for rendering.
  */
void Graphics_addDrawable(Drawable* d);

/** Terminate the graphics component of the engine.
  */
void Graphics_terminate();

/** Fill a rectangle with a color.
  *
  * @param rect The rectangle to fill, in world coordinates.
  * @param color The color to use for the fill.
  * @internal Haven't been able to test if any of this works. Need tests directory NAO.
  */
void Graphics_fillRect(Rectangle* rect, Color* color);


#endif // SMUG_GRAPHICS_GRAPHICS_H
