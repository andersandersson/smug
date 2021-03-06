/**
 * @file graphics.h
 * @brief Contains functions to control graphics.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @defgroup smug_graphics Graphics
 * The Graphics module handles all graphics in Smug.
 * @{
 */

#ifndef SMUG_GRAPHICS_GRAPHICS_H
#define SMUG_GRAPHICS_GRAPHICS_H

#include <common/common.h>
#include <utils/rectangle_type.h>
#include <utils/vector_type.h>
#include <graphics/color_type.h>
#include <graphics/drawable/drawable_type.h>
#include <graphics/camera.h>

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
 * This creates a view, mapping a screen to the current window.
 * The width and height are read from the platform module.
 */
int Graphics_init(void);

/**
 * Returns true if system is initialized
 */
BOOL Graphics_isInitialized(void);

/**
 * For setting the window size internally. This function should not be visible to user code.
 */
void Graphics_setWindowSize(double w, double h);

/**
 * Render one frame.
 */
void Graphics_render(void);

/**
 * Registers a drawable for rendering.
 */
void Graphics_addDrawable(Drawable* d);

/**
 * Removes a drawable from graphics engine.
 */
void Graphics_removeDrawable(Drawable* d);

/**
 * Terminate the graphics component of the engine.
 */
void Graphics_terminate(void);

/**
 * Gets the current camera
 */
Camera* Graphics_getCamera(void);

/**
 * Sets the properties of a layer
 * @param id the layer id
 * @param parallax value
 */
void Graphics_setupLayer(unsigned int id, float parallax);

/**
 * Sets the current rendering mode
 * @param flags A combination of rendering modes
 */
void Graphics_setRenderMode(unsigned int flags);

#endif // SMUG_GRAPHICS_GRAPHICS_H

/**@}*/
