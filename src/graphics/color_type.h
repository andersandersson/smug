/**
 * @file color_type.h
 * @brief Defines the Color type.
 * @ingroup smug_graphics
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_COLOR_TYPE_H
#define SMUG_GRAPHICS_COLOR_TYPE_H

/**
 * An RGBA Color type.
 */
typedef struct Color
{
    float r;
    float g;
    float b;
    float a;
} Color;
// typedef unsigned int Color;

#endif // SMUG_GRAPHICS_COLOR_TYPE_H

/**@}*/
