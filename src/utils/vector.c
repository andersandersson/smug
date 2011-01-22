#include <smugstd.h>

#include <common/common.h>

#include <utils/vector.h>

Vector Vector_create2d(float x, float y)
{
    Vector v;

    v.size = 2;

    v.d[0] = x;
    v.d[1] = y;

    return v;
}


Vector* Vector_new2d(float x, float y)
{
    Vector* newVector = (Vector*)malloc(sizeof(Vector));
    *newVector = Vector_create2d(x, y);
    return newVector;
}

void Vector_delete2d(void* self)
{
    free((Vector*)self);
}

Vector Vector_crossProduct2d(Vector v)
{
    return Vector_create2d(-v.d[1], v.d[0]);
}


void Vector_setX(Vector* v, float x)
{
    v->d[0] = x;
}

void Vector_setY(Vector* v, float y)
{
    v->d[1] = y;
}
float Vector_getX(Vector* v)
{
    return v->d[0];
}
float Vector_getY(Vector* v)
{
    return v->d[1];
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

Vector Vector_multiplyVector(Vector v, Vector w)
{
    int i;
    Vector u;
    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.d[i] = w.d[i]*v.d[i];
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
    float length = sqrt(Vector_dotProduct(v, v));

    u.size = v.size;

    for(i=0; i < v.size; i++)
    {
        u.d[i] = v.d[i] / length;
    }

    return u;
}


Vector Vector_projectionPlane(Vector v, Vector n)
{
    return Vector_sub(v, Vector_multiply(n, Vector_dotProduct(v, n)));
}


Vector Vector_projection(Vector v, Vector w)
{
  return Vector_multiply(w, Vector_dotProduct(v, w) / Vector_squareLength(w));
}


float Vector_length(Vector v)
{
    return sqrt(Vector_dotProduct(v, v));
}

float Vector_squareLength(Vector v)
{
    return Vector_dotProduct(v, v);
}

BOOL Vector_equal(Vector self, Vector other)
{
    static int i;
    if (self.size != other.size)
    {
        return FALSE;
    }
    for (i = 0; i < self.size; i++)
    {
        if (self.d[i] != other.d[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

void Vector_print(Vector v)
{
    int i;

    smug_printf("[");

    for(i=0; i < v.size; i++) {
        smug_printf("%.2f", v.d[i]);

        if(v.size - i > 1) {
            smug_printf(", ");
        }
    }

    smug_printf("]\n");
}
