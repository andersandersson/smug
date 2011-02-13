/**
 * @file point.h
 * @brief Defines the point functions.
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_POINT_H
#define SMUG_UTILS_POINT_H

#include <common/common.h>
#include <utils/vector_type.h>

#include <utils/point_type.h>

/**
 * Create a new Point
 * @relates Point
 */
Point* Point_new(void);

/**
 * Create a Point from coordinates
 * @relates Point
 */
Point Point_createFromXY(float x, float y);

/**
 * Create a Point from vector
 * @relates Point
 */
Point Point_createFromVector(Vector v);

/**
 * Delete a Point struct created with Point_new
 * @relates Point
 */
void Point_delete(Point* p);

/**
 * Add Vector to Point and return a new Point with the result.
 * @relates Point
 */
Point Point_addVector(Point p, Vector v);

/**
 * Return the distance between p1 and p2 as a new vector.
 * @relates Point
 */
Vector Point_distanceToPoint(Point p1, Point p2);

/**
 * Get the point that is a certain fraction of the way from point 1 to point 2.
 * @relates Point
 */
Point Point_linearInterpolate(Point p1, Point p2, float fraction);

/**
 * Add the values of two points together.
 * @relates Point
 */
Point Point_add(Point p1, Point p2);

/**
 * Get a vector pointing to the point.
 * @relates Point
 */
Vector Point_getVector(Point self);

/**
 * Check if two points are equal.
 * @relates Point
 */
BOOL Point_equal(Point self, Point other);

/**
 * Get the X component of a Point
 * @relates Point
 * @param p pointer to a Point
 */
float Point_getX(Point self);

/**
 * Get the Y component of a Point
 * @relates Point
 * @param p pointer to a Point
 */
float Point_getY(Point self);

/**
 * Set the X component of a Point
 * @relates Point
 * @param p pointer to a Point
 * @param x x-value
 */
void Point_setX(Point* p, float x);

/**
 * Set the Y component of a Point
 * @relates Point
 * @param p pointer to a Point
 * @param y y-value
 */
void Point_setY(Point* p, float y);

#endif // SMUG_UTILS_POINT_H

/**@}*/
