/**
 * @file rectangle.h
 * @brief Defines the rectangle functions.
 * @ingroup smug_utils
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef UTILS_RECTANGLE_H
#define UTILS_RECTANGLE_H

#include <utils/rectangle_type.h>

/**
 * Create a new Rectangle
 * @relates Rectangle
 */
Rectangle* Rectangle_new(void);

/**
 * Create a Rectangle from position and size
 * @relates Rectangle
 */
Rectangle Rectangle_createFromXYWH(float x, float y, float w, float h);

/**
 * Delete a Rectangle created with Rectangle_new
 * @relates Rectangle
 */
void Rectangle_delete(Rectangle* r);

/**
 * Get the size of the rectangle (as a diagonal vector).
 * @relates Rectangle
 */
Vector Rectangle_getSize(Rectangle self);

/**
 * Get the X axis position of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getX(Rectangle* r);

/**
 * Get the Y axis position of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getY(Rectangle* r);

/**
 * Get the width of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getW(Rectangle* r);

/**
 * Get the height of the rectangle.
 * @relates Rectangle
 */
float Rectangle_getH(Rectangle* r);

/**
 * Add a vector to the position of the rectangle.
 * @relates Rectangle
 */
void Rectangle_addVector(Rectangle* self, Vector v);

/**
 * Multiply the rectangles coordinates and size by a vector.
 * Used for scaling a rectangle.
 * @relates Rectangle
 */
Rectangle Rectangle_multiplyVector(Rectangle r, Vector s);

#endif // UTILS_RECTANGLE_H

/**@}*/
