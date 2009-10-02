/** @file graphics.h
  * @brief Contains functions to control graphics.
  */

#ifndef SMUG_GRAPHICS_GRAPHICS_H
#define SMUG_GRAPHICS_GRAPHICS_H

#include "color.h"
#include "common/common.h"
#include "utils/rectangle.h"
#include "utils/vector.h"
#include "drawable/drawable.h"
#include "graphics/camera.h"


// Predefined rendering modes
#define RENDER_NONE             0x0000  // nothing
#define RENDER_FX               0x0001  // shader effects
#define RENDER_LAYERS           0x0002  // layers
#define RENDER_CONSOLE          0x0004  // console
#define RENDER_NORMAL           0x000F  // normal mode
#define RENDER_DEBUG_GRID       0x0010  // coordinate grid
#define RENDER_DEBUG_PHYSICS    0x0020  // physics bodies
#define RENDER_DEBUG_GRAPHICS   0x0040  // wireframe
#define RENDER_DEBUG_FX         0x0080  // shader debug data
#define RENDER_DEBUG_SOUND      0x0100  // visualize sound sources
#define RENDER_DEBUG_TEXT       0x0200  // textual information
#define RENDER_DEBUG_IMPULSES   0x0400  // visualize impulses and forces
#define RENDER_DEBUG_ALL        0x0FF0  // debug mode
#define RENDER_FPS              0x1000  // show fps  
#define RENDER_ALL              0xFF    // render everything


/**
 * Initialize Graphics subsystem
 *
 * This creates a view, mapping the given width
 * and height to the current window.
 * @param width The width of the view in pixels.
 * @param height The height of the view in pixels.
 */
int Graphics_init(int width, int height);

/**
 * Returns true if system is initialized
 */
BOOL Graphics_isInitialized();

/**
 * Returns the current size of the screen
 */
Vector Graphics_getScreenSize(); 

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

/**
 * Sets the current rendering mode
 * @param flags A combination of rendering modes
 */
void Graphics_setRenderMode(unsigned int flags);

#endif // SMUG_GRAPHICS_GRAPHICS_H
