#ifndef SMUG_GRAPHICS_COLOR_H
#define SMUG_GRAPHICS_COLOR_H

/** An RGBA Color type.
  */
typedef struct Color
{
    float r;
    float g;
    float b;
    float a;
} Color;

/** Create a Color struct
  *
  * @relatesalso Color
  */
Color* Color_new(float r, float g, float b, float a);

/** Detele a Color struct
  *
  * @relatesalso Color
  */
void Color_delete(Color* c);

Color Color_create();

Color Color_createFromRgba(float r, float g, float b, float a);

#endif // SMUG_GRAPHICS_COLOR_H
