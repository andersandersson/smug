#include <smugstd.h>
#include "drawable.h"

#include "graphics/graphics.h"

#include "common/common.h"
#include <stdlib.h>

Drawable* Drawable_new(unsigned int vertexcount)
{
    Drawable* ret = (Drawable*)malloc(sizeof(Drawable));
    ret->layer = 0;
    ret->pos = Interpoint_new(Point_createFromXY(0, 0));
    ret->relativePos = Interpoint_new(Point_createFromXY(0, 0));
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
    Interpoint_delete(((Drawable*)d)->pos);
    Interpoint_delete(((Drawable*)d)->relativePos);
    free(d);
}

void Drawable_setPos(Drawable* d, Point pos)
{
    smug_assert(NULL != d);
    Interpoint_setTo(d->pos, pos);
    d->followObject = FALSE;
}

void Drawable_moveTo(Drawable* self, Point pos)
{
    smug_assert(NULL != self);
    Interpoint_moveTo(self->pos, pos);
    self->followObject = FALSE;
}

void Drawable_setPosRelative(Drawable* d, Point pos)
{
    smug_assert(NULL != d);
    Interpoint_setTo(d->relativePos, pos);
    d->followObject = TRUE;
}

void Drawable_moveToRelative(Drawable* d, Point pos)
{
    smug_assert(NULL != d);
    Interpoint_moveTo(d->relativePos, pos);
    d->followObject = TRUE;
}

Point Drawable_getPosForDrawing(Drawable* self)
{
    if (self->followObject)
    {
        if (self->parent != NULL)
        {
            return Point_add(Interpoint_getInterpolated(self->relativePos), GameObject_getPosForDrawing(self->parent));
        }
        else
        {
            return Interpoint_getInterpolated(self->relativePos);
        }
    }
    else
    {
        return Interpoint_getInterpolated(self->pos);
    }
}

void Drawable_commitPosition(Drawable* self)
{
    Interpoint_commit(self->pos);
    Interpoint_commit(self->relativePos);
}

void Drawable_setParent(Drawable* self, GameObject* parent)
{
    smug_assert(self != NULL);
    self->parent = parent;
}

void Drawable_removeParent(Drawable* self)
{
    smug_assert(self != NULL);
    self->parent = NULL;
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
