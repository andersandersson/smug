/** @file point.h
  * @brief Defines the point functions.
  */

#ifndef PHYSICS_POINT_H
#define PHYSICS_POINT_H

#include <common/common.h>

#include "utils/vector_type.h"
#include "utils/point_type.h"

/** Create a new Point
  *
  * @relatesalso Point
  */
Point* Point_new(void);

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
  * @relatesalso Point
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

Point Point_linearInterpolate(Point p1, Point p2, float fraction);

Point Point_add(Point p1, Point p2);

Vector Point_getVector(Point self);

BOOL Point_equal(Point self, Point other);

/**
 * Get the X component of a Point
 * @param p pointer to a Point
 */
float Point_getX(Point self);
/**
 * Get the Y component of a Point
 * @param p pointer to a Point
 */
float Point_getY(Point self);

/**
 * Set the X component of a Point
 * @param p pointer to a Point
 * @param x x-value
 */
void Point_setX(Point* p, float x);

/**
 * Set the Y component of a Point
 * @param p pointer to a Point
 * @param y y-value
 */
void Point_setY(Point* p, float y);

#endif // PHYSICS_POINT_H

