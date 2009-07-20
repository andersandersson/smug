#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"

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

/** 
 * Close down Graphics subsystem
 */
void Graphics_terminate();

/** 
 * Refresh graphics
 *
 * This should be called once per visual frame.
 */
void Graphics_refresh();


#endif // GRAPHICS_H

