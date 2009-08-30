/** @file drawable.h
  * @brief Defines the Drawable type.
  */

#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include "graphics/color.h"
#include "graphics/renderer/renderbatch.h"

/** Something that can be rendered to the screen. A superclass for several different drawable types.
  *
  */
typedef struct Drawable
{
    int layer;
    //Texture texture; <- can be NULL for shapes
    void (*_deleteFunc)(void* obj);
    int (*_writeBatchFunc)(struct Drawable* d, RenderBatch* batch, unsigned int start);
    
} Drawable;

void Drawable_delete(void* d);
int Drawable_writeBatchData(Drawable* d, RenderBatch* batch, unsigned int start);

#endif //SMUG_GRAPHICS_DRAWABLE_H
