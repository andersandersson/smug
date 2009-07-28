#include "color.h"

#include "common/common.h"
#include <stdlib.h>

Color* Color_new(float r, float g, float b, float a)
{
    Color* c = malloc(sizeof(Color));
    c->r = r;
    c->g = g;
    c->b = b;
    c->a = a;
    return c;
}

void Color_delete(Color* c)
{
    assert(NULL != c);
    free(c);
}

Color Color_create()
{
    Color c;
    c.r = 0.0f;
    c.g = 0.0f;
    c.b = 0.0f;
    c.a = 0.0f;
    return c;
}

Color Color_createFromRgba(float r, float g, float b, float a)
{
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}
