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

/** Create a Rectangle struct (on the stack)
  *
  * @relatesalso Rectangle
  */
Rectangle Rectangle_(double x, double y, double w, double h);

/** Create a Rectangle struct (on the heap)
  *
  * @relatesalso Rectangle
  */
Rectangle* Rectangle_new(double x, double y, double w, double h);

#endif // RECTANGLE_H
