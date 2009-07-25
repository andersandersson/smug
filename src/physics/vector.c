#include "vector.h"

#include <stdio.h>
#include <math.h>

Vector Vector_Create2d(double x, double y)
{
    Vector v;

    v.size = 2;

    v.x[0] = x;
    v.x[1] = y;

    return v;
}


Vector Vector_CrossProduct2d(Vector v)
{
    return Vector_Create2d(-v.x[1], v.x[0]);
}


Vector Vector_Add(Vector v, Vector w)
{
    int i;
    Vector u;
    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.x[i] = v.x[i] + w.x[i];
    }

    return u;
}


Vector Vector_Sub(Vector v, Vector w)
{
    int i;
    Vector u;
    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.x[i] = v.x[i] - w.x[i];
    }

    return u;
}


Vector Vector_Multiply(Vector v, double k)
{
    int i;
    Vector u;
    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.x[i] = k*v.x[i];
    }

    return u;
}


double Vector_DotProduct(Vector v, Vector w)
{
    int i;
    double m = 0;

    for(i=0; i < v.size; i++)
    {
        m += v.x[i] * w.x[i];
    }

    return m;
}


Vector Vector_Normalize(Vector v)
{
    Vector u;
    int i;
    double size = sqrt(Vector_DotProduct(v, v));

    u.size = v.size;
  
    for(i=0; i < v.size; i++) 
    {
        u.x[i] = v.x[i] / size;
    }

    return u;
}


Vector Vector_Projection(Vector v, Vector n)
{
    return Vector_Sub(v, Vector_Multiply(n, Vector_DotProduct(v, n)));
}

void Vector_Print(Vector v)
{
    int i;

    fprintf(stderr, "[");
  
    for(i=0; i < v.size; i++) {
        fprintf(stderr, "%.2f", v.x[i]);

        if(v.size - i > 1) {
            fprintf(stderr, ", ");
        }
    }

    fprintf(stderr, "]\n");
}
