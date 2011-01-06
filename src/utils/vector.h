/** @file vector.h
  * @brief Defines the Vector functions.
  * @ingroup smug_utils
  */

/** @addtogroup smug_utils
  * @{
  */

#ifndef VECTOR_H
#define VECTOR_H

#include <common/common.h>
#include <utils/vector_type.h>


// 2 dimensional specific functions
// -------------------------------


/**
 * Create a new 2 dimensional vector.
 */
Vector Vector_create2d(float x, float y);


/**
 * Calculate the normal (2 dimensional cross product)
 * for the Vector.
 */
Vector Vector_crossProduct2d(Vector v);


/**
 * Set the X coordinate of the vector
 */
void Vector_setX(Vector* v, float x);


/**
 * Set the Y coordinate of the vector
 */
void Vector_setY(Vector* v, float y);


/**
 * Get the X coordinate of the vector
 */
float Vector_getX(Vector* v);


/**
 * Get the Y coordinate of the vector
 */
float Vector_getY(Vector* v);


// Dimension independent functions
// -------------------------------


/**
 * Add to Vectors elements and return a new
 * Vector with the result.
 */
Vector Vector_add(Vector v, Vector w);


/**
 * Subtract Vector w:s elements from Vector
 * v:s and return result in a new Vector.
 */
Vector Vector_sub(Vector v, Vector w);


/**
 * Multiply the elements in v by k and
 * return a new Vector with the result.
 */
Vector Vector_multiply(Vector v, float k);

/**
 * Multiply Vectors v:s elements with vector w:s
 * and return a new Vector with the result.
 */
Vector Vector_multiplyVector(Vector v, Vector w);

/**
 * Calculate the dot product. This is the sum
 * v[1]*w[1] + ... + v[n]*w[n].
 */
float Vector_dotProduct(Vector v, Vector w);


/**
 * Normalize a Vector and return the result as
 * a new Vector.
 */
Vector Vector_normalize(Vector v);


/**
 * Calculate the projection of the Vector v
 * in the plane specified by the normal n.
 */
Vector Vector_projection(Vector v, Vector n);


/**
 * Calculate the length of the Vector v.
 */
float Vector_length(Vector v);

/**
 * Calculate the square length of the Vector v.
 */
float Vector_squareLength(Vector v);

BOOL Vector_equal(Vector self, Vector other);

// Miscellaneous vector functions
// ------------------------------

/**
 * Print the vector to stderr
 */
void Vector_print(Vector v);

#endif

/**@}*/
