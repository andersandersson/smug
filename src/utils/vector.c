#include "vector.h"

#include <stdio.h>
#include <math.h>

Vector Vector_create2d(float x, float y)
{
    Vector v;

    v.size = 2;

    v.d[0] = x;
    v.d[1] = y;

    return v;
}


Vector Vector_crossProduct2d(Vector v)
{
    return Vector_create2d(-v.d[1], v.d[0]);
}


Vector Vector_add(Vector v, Vector w)
{
    int i;
    Vector u;
    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.d[i] = v.d[i] + w.d[i];
    }

    return u;
}


Vector Vector_sub(Vector v, Vector w)
{
    int i;
    Vector u;
    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.d[i] = v.d[i] - w.d[i];
    }

    return u;
}


Vector Vector_multiply(Vector v, float k)
{
    int i;
    Vector u;
    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.d[i] = k*v.d[i];
    }

    return u;
}


float Vector_dotProduct(Vector v, Vector w)
{
    int i;
    double m = 0;

    for(i=0; i < v.size; i++)
    {
        m += v.d[i] * w.d[i];
    }

    return m;
}


Vector Vector_normalize(Vector v)
{
    Vector u;
    int i;
    float size = sqrt(Vector_dotProduct(v, v));

    u.size = v.size;
  
    for(i=0; i < v.size; i++) 
    {
        u.d[i] = v.d[i] / size;
    }

    return u;
}


Vector Vector_projection(Vector v, Vector n)
{
    return Vector_sub(v, Vector_multiply(n, Vector_dotProduct(v, n)));
}

void Vector_print(Vector v)
{
    int i;

    fprintf(stderr, "[");
  
    for(i=0; i < v.size; i++) {
        fprintf(stderr, "%.2f", v.d[i]);

        if(v.size - i > 1) {
            fprintf(stderr, ", ");
        }
    }

    fprintf(stderr, "]\n");
}