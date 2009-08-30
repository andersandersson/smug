/** @file rectangle.h
  * @brief Defines a rectangle type.
  */

#ifndef PHYSICS_RECTANGLE_H
#define PHYSICS_RECTANGLE_H

#include "utils/vector.h"

/** A simple rectangle type.
  *
  */
typedef struct Rectangle
{
    float x;
    float y;
    float w;
    float h;
} Rectangle;

/** Create a Rectangle struct
  *
  * @relatesalso Rectangle
  */
Rectangle* Rectangle_new(float x, float y, float w, float h);

/** Delete a Rectangle struct created with Rectangle_new
  *
  * @relatesalso Rectangle
  */
void Rectangle_delete(Rectangle* r);

#endif // PHYSICS_RECTANGLE_H
