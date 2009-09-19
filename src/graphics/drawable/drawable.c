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

void Drawable_writeBatchData(Drawable* d, BatchData* batchdata, unsigned int start)
{
    assert(NULL != d);
    if (NULL != ((Drawable*)d)->_writeBatchDataFunc)
        ((Drawable*)d)->_writeBatchDataFunc(d, batchdata, start);
}

int Drawable_getDataSize(Drawable* d)
{
    assert(NULL != d);
    if (NULL != ((Drawable*)d)->_getDataSizeFunc)
        return ((Drawable*)d)->_getDataSizeFunc(d);
        
    return 0;
}

void Drawable_delete(void* d)
{
    assert(NULL != d);
    free(((Drawable*)d)->vertices);
    free(d); 
}

void Drawable_setPos(Drawable* d, Point pos)
{
    assert(NULL != d);
    d->pos = pos;
}

void Drawable_setSprite(Drawable* d, Sprite* sprite)
{
    assert(NULL != d);
    d->sprite = sprite;
}

void Drawable_setLayer(Drawable* d, unsigned int layer)
{
    assert(NULL != d);
    d->layer = layer;
}

void Drawable_setColor(Drawable* d, Color color)
{
	assert(NULL != d);
	d->color = color;
}


unsigned int Drawable_getLayer(Drawable* d)
{
    assert(NULL != d);
    return d->layer;
}
Texture* Drawable_getTexture(Drawable* d)
{
    assert(NULL != d);
    if (NULL != d->sprite)
    {
        return d->sprite->texture;
    }
    return NULL;
}

unsigned int Drawable_getTextureID(Drawable* d)
{
    assert(NULL != d);
    if (NULL != d->sprite)
    {
        return d->sprite->texture->texid;
    }
    return 0;
}

unsigned int Drawable_getObjectSize(Drawable* d)
{
    assert(NULL != d);
    return d->vertexcount;
}

