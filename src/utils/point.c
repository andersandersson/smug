#include "rectangle.h"

#include "common/common.h"
#include <stdlib.h>

static Point sp;

Point* Point_new()
{
    Point* p = malloc(sizeof(Point));
    p->v = Vector_create2d(0,0);
    return p;
}

Point Point_createFromXY(float x, float y)
{
    sp.v = Vector_create2d(x,y);
    return sp;
}

Point Point_createFromVector(Vector v)
{
    sp.v = v;
    return sp;
}

float Point_getX(Point* p)
{
    return p->v.d[0];
}
float Point_getY(Point* p)
{
    return p->v.d[1];
}

void Point_setX(Point* p, float x)
{
    p->v.d[0] = x;
}
void Point_setY(Point* p, float y)
{
    p->v.d[1] = y;
}

void Point_delete(Point* p)
{
    free(p);
}


Point Point_addVector(Point p, Vector v)
{
    return Point_createFromVector(Vector_add(p.v, v));
}

Vector Point_distanceToPoint(Point p1, Point p2)
{
    return Vector_create2d(p2.v.d[0] - p1.v.d[0], p2.v.d[1] - p1.v.d[1]);
}
