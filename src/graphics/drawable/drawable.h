/** @file drawable.h
  * @brief Defines the Drawable type.
  * @ingroup smug_graphics
  */

/** @addtogroup smug_graphics
  * @{
  */

#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include "graphics/color.h"
#include "graphics/sprite.h"
#include "graphics/renderer/batchdata.h"

#include "utils/point.h"
#include "utils/vector.h"

/** 
  * A graphical entity on screen. A Drawable is a common interface
  * for a collection of different graphic components.
  */
typedef struct Drawable
{
    int type;
    unsigned int layer;
    Point pos;
    Sprite* sprite; // Is NULL for shapes
    Color color;
    unsigned int vertexcount;
    Vector* vertices;
    
    void (*_writeBatchDataFunc)(struct Drawable* d, BatchData* batch, unsigned int start); // Function for writing data
    int (*_getDataSizeFunc)(struct Drawable* d); // Function for getting data size
    
} Drawable;

/**
 * Creates new drawable
 * @param vertexcount size of drawable in vertices
 * @return Drawable
 */
Drawable* Drawable_new(unsigned int vertexcount);

/**
 * Deletes a Drawable
 * @param d Drawable to be deleted
 */
void Drawable_delete(void* d);

/**
 * Write all vertex data to batchdata
 * @param d Drawable containing data
 * @param batchdata batch for data to be written to
 * @param start where to start writing in batch
 */
void Drawable_writeBatchData(Drawable* d, BatchData* batchdata, unsigned int start);

/**
 * Get the size of the data that will be written
 * @param d Drawable
 * @return size of data in vertices
 */
int Drawable_getDataSize(Drawable* d);

/**
 * Set position of a Drawable
 * @param d the Drawable
 * @param pos Position 
 */
void Drawable_setPos(Drawable* d, Point pos);

/**
 * Set sprite of a Drawable
 * @param d the Drawable
 * @param sprite a pointer to a Sprite
 */
void Drawable_setSprite(Drawable* d, Sprite* sprite);

/**
 * Set the graphical layer of a drawable
 * @param d Drawable
 * @param layer the layer
 */
void Drawable_setLayer(Drawable* d, unsigned int layer);

/**
 * Set color of a Drawable
 * @param d Drawable
 * @param color Color
 */
void Drawable_setColor(Drawable* d, Color color);

/**
 * Get the layer of a Drawable
 * @param d the Drawable
 * @return layer id
 */
unsigned int Drawable_getLayer(Drawable* d);

/**
 * Get the texture of a drawable as pointer
 * @param d the Drawable
 * @return Texture
 */
Texture* Drawable_getTexture(Drawable* d);

/**
 * Get texture of a drawable as an id
 * @param d the Drawable
 * @return texture id
 */
unsigned int Drawable_getTextureID(Drawable* d);

/**
 * Get the size of an element of the drawable.
 * This size is the size of each component of a drawable, 
 * if a drawable consists of several rectangles (such as a font) 
 * then the objectsize is the number of vertices in one rectangle.
 * @param d the Drawable
 * @return 
 */
unsigned int Drawable_getObjectSize(Drawable* d);

#endif //SMUG_GRAPHICS_DRAWABLE_H

/**@}*/
