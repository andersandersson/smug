#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H

#include <graphics/drawable/drawableshape_type.h>

struct Shape;
struct BatchData;

BOOL DrawableShape_getShape(DrawableShape* self, struct Shape** shape);
BOOL DrawableShape_setShape(DrawableShape* self, struct Shape* shape);

void DrawableShape_deInit(DrawableShape* self);
void DrawableShape_init(DrawableShape* self);

#endif // SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H
