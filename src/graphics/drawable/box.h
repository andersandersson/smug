#ifndef GRAPHICS_DRAWABLE_BOX_H
#define GRAPHICS_DRAWABLE_BOX_H


#include "drawable.h"
#include "graphics/renderer/renderbatch.h"
#include "utils/rectangle.h"
#include "utils/vector.h"

// Inherits Drawable 
typedef struct Box
{
    Drawable base;
    Rectangle rect;
    Color color;
} Box;

Box* Box_new();
Box* Box_newFromPointRectangle(Point pos, Rectangle rect);

void Box_delete(void* box);
int Box_writeBatchData(Drawable* box, RenderBatch* batch, unsigned int start);

#endif //GRAPHICS_DRAWABLE_BOX_H
