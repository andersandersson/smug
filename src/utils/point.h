/** @file point.h
  * @brief Defines a rectangle type.
  */

#ifndef PHYSICS_POINT_H
#define PHYSICS_POINT_H

#include "utils/vector.h"

/** A simple rectangle type.
  *
  */
typedef struct Point
{
    Vector v;
} Point;

/** Create a new Point
  *
  * @relatesalso Point
  */
Point* Point_new();

/** Create a Point from coordinates
  *
  * @relatesalso Point
  */
Point Point_createFromXY(float x, float y);

/** Create a Point from vector
  *
  * @relatesalso Point
  */
Point Point_createFromVector(Vector v);

/** Delete a Point struct created with Point_new
  *
  * @relatesalso Rectangle
  */
void Point_delete(Point* p);

/**
 * Add Vector to Point and return a new
 * Point with the result.
 */
Point Point_addVector(Point p, Vector v);

/**
 * Return the distance between p1 and p2 as
 * a new vector.
 */
Vector Point_distanceToPoint(Point p1, Point p2);


float Point_getX(Point* p);
float Point_getY(Point* p);

#endif // PHYSICS_POINT_H

