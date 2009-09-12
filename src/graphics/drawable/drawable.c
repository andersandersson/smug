#include "drawable.h"

#include "graphics.h"

#include "common/common.h"
#include <stdlib.h>

Drawable* Drawable_new(unsigned int vertexcount)
{
    Drawable* ret = (Drawable*)malloc(sizeof(Drawable));
    ret->layer = 0;
    ret->pos = Point_createFromXY(0, 0);
    ret->color = Color_create();
    ret->vertexcount = vertexcount;
    ret->vertices = (Vector*)malloc(sizeof(Vector) * vertexcount);
    ret->sprite = NULL;
    ret->type = 0;
    return ret;
}

int Drawable_writeBatchData(Drawable* d, RenderBatch* batch, unsigned int start)
{
    if (NULL != ((Drawable*)d)->_writeBatchFunc)
        return ((Drawable*)d)->_writeBatchFunc(d, batch, start);
        
    return 0;
}

void Drawable_delete(void* d)
{
    free(((Drawable*)d)->vertices);
    free(d); 
}

void Drawable_setPos(Drawable* d, Point pos)
{
    d->pos = pos;
}

void Drawable_setSprite(Drawable* d, Sprite* sprite)
{
    d->sprite = sprite;
}

void Drawable_setLayer(Drawable* d, int layer)
{
    d->layer = layer;
}
