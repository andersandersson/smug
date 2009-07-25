/** @file rectangle.h
  * @brief Defines a rectangle type.
  */

#ifndef RECTANGLE_H
#define RECTANGLE_H

/** A simple rectangle type.
  *
  */
typedef struct Rectangle
{
	double x;
	double y;
	double w;
	double h;
} Rectangle;

/** Create a Rectangle struct
  *
  * @relatesalso Rectangle
  */
Rectangle* Rectangle_new(double x, double y, double w, double h);

/** Delete a Rectangle struct created with Rectangle_new
  *
  * @relatesalso Rectangle
  */
void Rectangle_delete(Rectangle* r);

#endif // RECTANGLE_H
