#include "renderer.h"

#include "renderbatch.h"
#include "layer.h"

#include "stdlib.h"
#include "stdio.h"

#include "graphics/sprite.h"

extern int gVBOSupported;

Renderer* Renderer_new()
{
    Renderer* ret = (Renderer*)malloc(sizeof(Renderer));
    ret->layers = ArrayList_new();
    ret->layercount = 1;
    
    return ret;
}

void Renderer_delete(void* renderer)
{
    ArrayList_deleteContents(((Renderer*)renderer)->layers, Layer_delete);
    ArrayList_delete(((Renderer*)renderer)->layers);
}

void Renderer_render(Renderer* renderer)
{
    Layer* layer;
    int i;
    for (i = 0; i < renderer->layercount; i++)
    {
        layer = ArrayList_get(renderer->layers, i);
        Layer_render(layer);
    }
}

void Renderer_addDrawable(Renderer* renderer, Drawable* drawable)
{
    assert(NULL != renderer);
    assert(NULL != drawable);
    
    Layer* layer = ArrayList_get(renderer->layers, Drawable_getLayer(drawable));
    if (NULL == layer )
    {
        layer = Layer_new();
        ArrayList_set(renderer->layers, Drawable_getLayer(drawable), layer);
    }
    Layer_addDrawable(layer, drawable);  
}
