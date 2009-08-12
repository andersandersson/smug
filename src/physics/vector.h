#ifndef VECTOR_H
#define VECTOR_H

/**
 * Simple 2 dimensional vector.
 */
typedef struct Vector2d
{
    int size;    //!< The dimension of the vector
    float x[2]; //!< The elements of the vector
} Vector;


// 2 dimensinal specific functions
// -------------------------------


Vector* Vector_new();

Vector* Vector_new2f(float x, float y);

void Vector_delete(Vector* v);

float Vector_crossProduct2f(Vector* v, Vector* dest);


// Dimension independent functions 
// -------------------------------


void Vector_add(Vector* v, Vector* w, Vector* dest);

void Vector_sub(Vector* v, Vector* w, Vector* dest);

void Vector_multiply(Vector *v, float k, Vector* dest);

float Vector_dotProduct(Vector* v, Vector* w);

void Vector_normalize(Vector* v, Vector* dest);

void Vector_projection(Vector* v, Vector* n, Vector* dest);


void Vector_print(Vector* v);
#endif
