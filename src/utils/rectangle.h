/** @file rectangle.h
  * @brief Defines a rectangle type.
  */

#ifndef PHYSICS_RECTANGLE_H
#define PHYSICS_RECTANGLE_H

#include "utils/vector.h"
#include "utils/point.h"

/** A simple rectangle type.
  *
  */
typedef struct Rectangle
{
    Point pos;
    Vector size;
} Rectangle;

/** Create a new Rectangle
  *
  * @relatesalso Rectangle
  */
Rectangle* Rectangle_new(void);

/** Create a Rectangle from position and size
  *
  * @relatesalso Rectangle
  */
Rectangle Rectangle_createFromXYWH(float x, float y, float w, float h);

/** Delete a Rectangle created with Rectangle_new
  *
  * @relatesalso Rectangle
  */
void Rectangle_delete(Rectangle* r);

float Rectangle_getX(Rectangle* r);
float Rectangle_getY(Rectangle* r);
float Rectangle_getW(Rectangle* r);
float Rectangle_getH(Rectangle* r);

Rectangle Rectangle_multiplyVector(Rectangle r, Vector s);


#endif // PHYSICS_RECTANGLE_H
