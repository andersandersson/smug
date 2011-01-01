#include <smugstd.h>
#include "color.h"

#include "common/common.h"
#include <stdlib.h>

Color Color_create(void)
{
    Color c;
    c.r = 0.0f;
    c.g = 0.0f;
    c.b = 0.0f;
    c.a = 0.0f;
    return c;
}

Color Color_createFromRGBAf(float r, float g, float b, float a)
{
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

Color Color_createFromRGBAi(int r, int g, int b, int a)
{
    return Color_createFromRGBAf(_iTof(r), _iTof(g), _iTof(b), _iTof(a));
}

int Color_Ri(Color c)
{
    return _fToi(c.r);
}
int Color_Gi(Color c)
{
    return _fToi(c.g);
}
int Color_Bi(Color c)
{
    return _fToi(c.b);
}
int Color_Ai(Color c)
{
    return _fToi(c.a);
}

float Color_Rf(Color c)
{
    return c.r;
}
float Color_Gf(Color c)
{
    return c.g;
}
float Color_Bf(Color c)
{
    return c.b;
}
float Color_Af(Color c)
{
    return c.a;
}

Color Color_setCompAi(Color c, int a)
{
    return Color_setCompAf(c, _iTof(a));
}

Color Color_setCompAf(Color c, float a)
{
    c.a = a;
    return c;
}
