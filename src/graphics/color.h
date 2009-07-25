#ifndef COLOR_H
#define COLOR_H

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

#endif // COLOR_H
