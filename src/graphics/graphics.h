/** @file graphics.h
  * @brief Contains functions to control graphics.
  */

#ifndef SMUG_GRAPHICS_GRAPHICS_H
#define SMUG_GRAPHICS_GRAPHICS_H

#include "color.h"
#include "common/common.h"
#include "utils/rectangle.h"
#include "drawable/drawable.h"
#include "graphics/camera.h"

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

/** Gets the current camera
 */
Camera* Graphics_getCamera();

/** Sets the properties of a layer
 */
void Graphics_setupLayer(unsigned int id, float parallax);

#endif // SMUG_GRAPHICS_GRAPHICS_H
