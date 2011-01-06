/** @file rectangle.h
  * @brief Defines the rectangle functions.
  */

#ifndef UTILS_RECTANGLE_H
#define UTILS_RECTANGLE_H

#include <utils/rectangle_type.h>

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

Vector Rectangle_getSize(Rectangle self);

float Rectangle_getX(Rectangle* r);
float Rectangle_getY(Rectangle* r);
float Rectangle_getW(Rectangle* r);
float Rectangle_getH(Rectangle* r);

Rectangle Rectangle_multiplyVector(Rectangle r, Vector s);


#endif // UTILS_RECTANGLE_H
