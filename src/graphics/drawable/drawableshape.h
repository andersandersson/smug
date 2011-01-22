#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H

struct Shape;
struct BatchData;
struct GameObject;
struct DrawableShape;

BOOL DrawableShape_getShape(struct DrawableShape* self, struct Shape** shape);
BOOL DrawableShape_setShape(struct DrawableShape* self, struct Shape* shape);

void DrawableShape_deInit(struct DrawableShape* self);
void DrawableShape_init(struct DrawableShape* self);

#endif // SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H
