#include <smugstd.h>

#include <common/common.h>
#include <utils/rectangle.h>
#include <utils/vector.h>

#include <utils/point.h>

static Point sp;

Point* Point_new(void)
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

float Point_getX(Point self)
{
    return Vector_getX(&self.v);
}
float Point_getY(Point self)
{
    return Vector_getY(&self.v);
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

Point Point_linearInterpolate(Point p1, Point p2, float fraction)
{
    static float oldX;
    static float oldY;
    oldX = Point_getX(p1);
    oldY = Point_getY(p1);
    return Point_createFromXY(oldX + (Point_getX(p2) - oldX) * fraction,
                              oldY + (Point_getY(p2) - oldY) * fraction);
}

Point Point_add(Point p1, Point p2)
{
    return Point_createFromXY(Point_getX(p1) + Point_getX(p2), Point_getY(p1) + Point_getY(p2));
}

Vector Point_getVector(Point self)
{
    return self.v;
}

BOOL Point_equal(Point self, Point other)
{
    return Vector_equal(self.v, other.v);
}
