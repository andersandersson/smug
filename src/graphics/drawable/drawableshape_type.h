#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_TYPE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_TYPE_H

struct Shape;

#include <graphics/drawable/drawable_type.h>

typedef struct DrawableShape
{
	Drawable base; // Inherit from Drawable

    struct Shape* mShape;
    // Can shape be inherited???

} DrawableShape;

#endif /* SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_TYPE_H */
