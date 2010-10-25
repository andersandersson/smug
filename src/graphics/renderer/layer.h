#ifndef SMUG_GRAPHICS_LAyER_H
#define SMUG_GRAPHICS_LAYER_H

#include "common/common.h"

#include "graphics/drawable/drawable.h"

#include "utils/arraylist.h"
#include "utils/linkedlist.h"

/*
 * A layer consists of a list of drawables plus a two-dimensional array
 * of batches. First array-component is the size of the drawables in
 * vertexes. second component is which texture the drawables use, if any.
*/

typedef struct Layer
{
    ArrayList* batcharrays;  // Arrays of batches sorted by object size
    float parallax;
} Layer;

Layer* Layer_new(void);
void Layer_delete(void* layer);

void Layer_addDrawable(Layer* layer, Drawable* drawable);

void Layer_render(Layer* layer);
float Layer_getParallax(Layer* layer);
void Layer_setParallax(Layer* layer, float value);

#endif //SMUG_GRAPHICS_LAYER_H
