#ifndef SMUG_GRAPHICS_INTERFACE_H
#define SMUG_GRAPHICS_INTERFACE_H

#include <utils/interface.h>
#include <graphics/color_type.h>

/* SmugSprite* getSpriteFromFile(char* fileName); */

struct Drawable;
typedef struct Drawable* SmugDrawable;

typedef Color SmugColor;

SmugDrawable smugDrawable_newFromRect(SmugRect rect);
void smugDrawable_setColor(SmugDrawable drawable, SmugColor color);
SmugColor smugColor_create(int r, int g, int b, int a);
/* SmugDrawable* smugDrawable_fromSprite(SmugSprite* sprite); */
/* SmugDrawable* smugDrawable_fromShape(SmugShape* shape); */

#endif // SMUG_GRAPHICS_INTERFACE_H
