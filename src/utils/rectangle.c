#include "rectangle.h"

#include "common/common.h"
#include <utils/point.h>
#include <utils/vector.h>
#include <stdlib.h>

static Rectangle newRect;

Rectangle* Rectangle_new(void)
{
    Rectangle* r = malloc(sizeof(Rectangle));
    r->pos = Point_createFromXY(0,0);
    r->size = Vector_create2d(0,0);
    return r;
}

Rectangle Rectangle_createFromXYWH(float x, float y, float w, float h)
{
    newRect.pos = Point_createFromXY(x,y);
    newRect.size = Vector_create2d(w,h);
    return newRect;
}

Vector Rectangle_getSize(Rectangle self)
{
    return self.size;
}

float Rectangle_getX(Rectangle* r)
{
    return Point_getX(r->pos);
}
float Rectangle_getY(Rectangle* r)
{
    return Point_getY(r->pos);
}
float Rectangle_getW(Rectangle* r)
{
    return r->size.d[0];
}
float Rectangle_getH(Rectangle* r)
{
    return r->size.d[1];
}

void Rectangle_addVector(Rectangle* self, Vector v)
{
    self->pos = Point_addVector(self->pos, v);
    self->size = Vector_add(self->size, v);
}

void Rectangle_delete(Rectangle* r)
{
    free(r);
}


Rectangle Rectangle_multiplyVector(Rectangle r, Vector s)
{
    return Rectangle_createFromXYWH(Point_getX(r.pos) * s.d[0], Point_getY(r.pos) * s.d[1], r.size.d[0] * s.d[0], r.size.d[1] * s.d[1]);
}
