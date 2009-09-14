#ifndef SMUG_GRAPHICS_RENDERER_H
#define SMUG_GRAPHICS_RENDERER_H

#include "utils/arraylist.h"
#include "graphics/drawable/drawable.h"


// A renderer renders one layer of graphics objects from an internal list of drawables.
// A renderer should be able to take z-order into consideration if configured as such.

typedef struct Renderer
{
    ArrayList* layers;
    unsigned int layercount;

} Renderer;

Renderer* Renderer_new();
void Renderer_delete(void* renderer);

void Renderer_render(Renderer* renderer);

void Renderer_addDrawable(Renderer* renderer, Drawable* drawable);














#endif //SMUG_GRAPHICS_RENDERER_H

