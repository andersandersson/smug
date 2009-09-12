/** @file drawable.h
  * @brief Defines the Drawable type.
  */

#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include "graphics/color.h"
#include "graphics/sprite.h"
#include "graphics/renderer/renderbatch.h"

#include "utils/point.h"
#include "utils/vector.h"

/** Something that can be rendered to the screen. A superclass for several different drawable types.
  *
  */
typedef struct Drawable
{
    int type;
    int layer;
    Point pos;
    Sprite* sprite; //<- can be NULL for shapes
    Color color;
    unsigned int vertexcount;
    Vector* vertices;
    
    int (*_writeBatchFunc)(struct Drawable* d, RenderBatch* batch, unsigned int start);
    
} Drawable;

Drawable* Drawable_new(unsigned int vertexcount);
void Drawable_delete(void* d);
int Drawable_writeBatchData(Drawable* d, RenderBatch* batch, unsigned int start);
void Drawable_setPos(Drawable* d, Point pos);
void Drawable_setSprite(Drawable* d, Sprite* sprite);
void Drawable_setLayer(Drawable* d, int layer);


#endif //SMUG_GRAPHICS_DRAWABLE_H
