/** @file drawable.h
  * @brief Defines the Drawable type.
  */

#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include "graphics/color.h"
#include "graphics/sprite.h"
#include "graphics/renderer/batchdata.h"

#include "utils/point.h"
#include "utils/vector.h"

/** Something that can be rendered to the screen. A superclass for several different drawable types.
  *
  */
typedef struct Drawable
{
    int type;
    unsigned int layer;
    Point pos;
    Sprite* sprite; //<- can be NULL for shapes
    Color color;
    unsigned int vertexcount;
    Vector* vertices;
    
    void (*_writeBatchDataFunc)(struct Drawable* d, BatchData* batch, unsigned int start);
    int (*_getDataSizeFunc)(struct Drawable* d);
    
} Drawable;

Drawable* Drawable_new(unsigned int vertexcount);
void Drawable_delete(void* d);
void Drawable_writeBatchData(Drawable* d, BatchData* batchdata, unsigned int start);
int Drawable_getDataSize(Drawable* d);

void Drawable_setPos(Drawable* d, Point pos);
void Drawable_setSprite(Drawable* d, Sprite* sprite);
void Drawable_setLayer(Drawable* d, unsigned int layer);
void Drawable_setColor(Drawable* d, Color color);

unsigned int Drawable_getLayer(Drawable* d);
Texture* Drawable_getTexture(Drawable* d);
unsigned int Drawable_getTextureID(Drawable* d);

unsigned int Drawable_getObjectSize(Drawable* d);

#endif //SMUG_GRAPHICS_DRAWABLE_H
