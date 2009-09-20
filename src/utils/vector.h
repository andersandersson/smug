#ifndef VECTOR_H
#define VECTOR_H

/**
 * Simple 2 dimensional vector.
 */
typedef struct Vector2d
{
    int size;    //!< The dimension of the vector
    float d[2];
} Vector;


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


// Miscellaneous vector functions
// ------------------------------

/**
 * Print the vector to stderr
 */
void Vector_print(Vector v);



#endif
