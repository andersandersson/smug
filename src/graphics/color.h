/** @file color.h
  * @brief Defines the Color type functions.
  * @ingroup smug_graphics
  */

/** @addtogroup smug_graphics
  * @{
  */

#ifndef SMUG_GRAPHICS_COLOR_H
#define SMUG_GRAPHICS_COLOR_H

#include <graphics/color_type.h>

Color Color_create(void);
// #define Color_create() (0);

Color Color_createFromRGBAf(float r, float g, float b, float a);
Color Color_createFromRGBAi(int r, int g, int b, int a);
// #define Color_createFromRGBAi(r, g, b, a) ((r << 24) | (g << 16) | (b << 8) | a)
// #define Color_createFromRGBAf(r, g, b, a) Color_createFromRGBAi(_fToi(r), _fToi(g), _fToi(b), _fToi(a))

#define _fToi(f) ((int)(255.0f * f))
#define _iTof(i) ((float)i / 255.0f)

// #define Color_Ri(c) ((c & 0xff000000) >> 24)
// #define Color_Gi(c) ((c & 0x00ff0000) >> 16)
// #define Color_Bi(c) ((c & 0x0000ff00) >> 8)
// #define Color_Ai(c) ((c & 0x000000ff))

// #define Color_Rf(c) _iTof(Color_Ri(c))
// #define Color_Gf(c) _iTof(Color_Gi(c))
// #define Color_Bf(c) _iTof(Color_Bi(c))
// #define Color_Af(c) _iTof(Color_Ai(c))

// #define Color_setCompAi(c, a) Color_createFromRGBAi(Color_Ri(c), Color_Gi(c), Color_Bi(c), a)
// #define Color_setCompAf(c, a) Color_setCompAi(c, _fToi(a))

int Color_Ri(Color c);
int Color_Gi(Color c);
int Color_Bi(Color c);
int Color_Ai(Color c);

float Color_Rf(Color c);
float Color_Gf(Color c);
float Color_Bf(Color c);
float Color_Af(Color c);

Color Color_setCompAi(Color c, int a);
Color Color_setCompAf(Color c, float a);

#endif // SMUG_GRAPHICS_COLOR_H

/**@}*/
