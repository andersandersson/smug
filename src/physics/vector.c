#include "vector.h"

#include "common/common.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>



Vector* Vector_new()
{
    return Vector_new2f(0.0, 0.0);
}

Vector* Vector_new2f(float x, float y)
{
    Vector* v = malloc(sizeof(Vector));
    
    v->size = 2;
    v->x[0] = x;
    v->x[1] = y;

    return v;
}

void Vector_delete(Vector* v)
{
    free(v);
}

float Vector_crossProduct2f(Vector* v, Vector* dest)
{
    float r;

    dest->x[0] = - v->x[1];
    dest->x[1] = v->x[0];

    r = (v->x[0]*v->x[0]) + (v->x[1]*v->x[1]);

    return r;
}

void Vector_add(Vector* v, Vector* w, Vector* dest)
{
    int i;

    assert(v->size == w->size);
    assert(v->size == dest->size);

    for(i=0; i<v->size; i++)
        {
            dest->x[i] = v->x[i] + w->x[i];
        }
}

void Vector_sub(Vector* v, Vector* w, Vector* dest)
{
}

void Vector_multiply(Vector *v, float k, Vector* dest)
{
}

float Vector_dotProduct(Vector* v, Vector* w)
{
    int i;
    float p = 0.0;

    assert(v->size == w->size);

    for(i=0; i<v->size; i++)
        {
            p += v->x[i] * w->x[i];
        }

    return p;
}

void Vector_normalize(Vector* v, Vector* dest)
{
}

void Vector_projection(Vector* v, Vector* n, Vector* dest)
{
}


void Vector_print(Vector* v)
{
    int i;

    fprintf(stderr, "(");
    for(i=0; i<v->size; i++)
        {
            fprintf(stderr, "%f", v->x[i]);

            if(v->size-i > 1)
                {
                    fprintf(stderr, ", ");
                }
        }
    fprintf(stderr, ")\n");
}
