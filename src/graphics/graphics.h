/** @file graphics.h
  * @brief Contains functions to control graphics.
  */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "color.h"
#include "common/common.h"
#include "utils/rectangle.h"

/**
 * Initialize Graphics subsystem
 *
 * This will create a window of a given size and set it
 * in either fullscreen or windowed mode.
 * @param width The width of the window in pixels.
 * @param height The height of the window in pixels.
 * @param fullscreen Whether or not to run in fullscreen mode.
 */
int Graphics_init(int width, int height, BOOL fullscreen);

/** Refresh the screen.
  */
void Graphics_refresh();

/** Terminate the graphics component of the engine.
  */
void Graphics_terminate();

/** Set the part of the world that will be visible in the window.
  * 
  * The Y-axis points upward.
  * @internal Okay, I admit I'm confused about the Y axis. Up or down?
  * @param rect A rectangle 
  */
void Graphics_setVisibleArea(Rectangle* rect);

/** Fill a rectangle with a color.
  *
  * @param rect The rectangle to fill, in world coordinates.
  * @param color The color to use for the fill.
  * @internal Haven't been able to test if any of this works. Need tests directory NAO.
  */
void Graphics_fillRect(Rectangle* rect, Color* color);


#endif // GRAPHICS_H
