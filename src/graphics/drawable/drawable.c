#include <smugstd.h>
#include "drawable.h"

#include "graphics/graphics.h"

#include "common/common.h"
#include <stdlib.h>

Drawable* Drawable_new(unsigned int vertexcount)
{
    Drawable* ret = (Drawable*)malloc(sizeof(Drawable));
    ret->layer = 0;
    ret->pos = Point_createFromXY(0, 0);
    ret->relativePos = ret->pos;
    ret->color = Color_create();
    ret->followObject = TRUE;
    ret->vertexcount = vertexcount;
    ret->vertexOffsets = (Vector*)malloc(sizeof(Vector) * vertexcount);
    ret->sprite = NULL;
    ret->type = 0;
    ret->parent = NULL;
    return ret;
}

void Drawable_writeBatchData(Drawable* d, BatchData* batchdata, unsigned int start)
{
    smug_assert(NULL != d);
    if (NULL != ((Drawable*)d)->_writeBatchDataFunc)
        ((Drawable*)d)->_writeBatchDataFunc(d, batchdata, start);
}

int Drawable_getDataSize(Drawable* d)
{
    smug_assert(NULL != d);
    if (NULL != ((Drawable*)d)->_getDataSizeFunc)
        return ((Drawable*)d)->_getDataSizeFunc(d);

    return 0;
}

void Drawable_delete(void* d)
{
    smug_assert(NULL != d);
    free(((Drawable*)d)->vertexOffsets);
    free(d);
}

void Drawable_setPos(Drawable* d, Point pos)
{
    smug_assert(NULL != d);
    d->pos = pos;
    d->followObject = FALSE;
}

void Drawable_updatePos(Drawable* self)
{
    if (self->parent)
        self->pos = Point_addVector(self->relativePos, GameObject_getPos(self->parent));
}

void Drawable_setPosRelative(Drawable* d, Point pos)
{
    smug_assert(NULL != d);
    d->relativePos = pos;
    if (d->parent)
        d->pos = Point_addVector(pos, GameObject_getPos(d->parent));
    else
        d->pos = pos;
    d->followObject = TRUE;
}

void Drawable_followObject(Drawable* self, BOOL follow)
{
    self->followObject = follow;
}

void Drawable_setSprite(Drawable* d, Sprite* sprite)
{
    smug_assert(NULL != d);
    d->sprite = sprite;
}

void Drawable_setLayer(Drawable* d, unsigned int layer)
{
    smug_assert(NULL != d);
    d->layer = layer;
}

void Drawable_setColor(Drawable* d, Color color)
{
	smug_assert(NULL != d);
	d->color = color;
}

void Drawable_setOpacity(Drawable* self, float opacity)
{
    self->color = Color_setCompAf(self->color, opacity);
}

unsigned int Drawable_getLayer(Drawable* d)
{
    smug_assert(NULL != d);
    return d->layer;
}

Texture* Drawable_getTexture(Drawable* d)
{
    smug_assert(NULL != d);
    if (NULL != d->sprite)
    {
        return d->sprite->texture;
    }
    return NULL;
}

unsigned int Drawable_getTextureID(Drawable* d)
{
    smug_assert(NULL != d);
    if (NULL != d->sprite)
    {
        return d->sprite->texture->texid;
    }
    return 0;
}

unsigned int Drawable_getObjectSize(Drawable* d)
{
    smug_assert(NULL != d);
    return d->vertexcount;
}
