#include "drawable.h"

#include "graphics.h"

#include "common/common.h"
#include <stdlib.h>

/*

static void Drawable_invariant(Drawable* d)
{
    // The drawable is not null.
    assert(NULL != d);
    
    // The Drawable either has a Sprite OR a Color and a Rectangle, but not both.
    if (NULL == d->sprite)  // No sprite, must have rectangle and color
    {
        assert(NULL != d->rect && NULL != d->color);
    }
    else                    // Has sprite, must not have rectangle or color.
    {
        assert(NULL == d->rect && NULL == d->color);
    }
}

Drawable* Drawable_newSprite(Sprite* s)
{
    assert(NULL != s);
    
    Drawable* d = malloc(sizeof(Drawable));
    d->sprite = s;
    d->rect = NULL;
    d->color = NULL;
    Drawable_invariant(d);
    return d;
}

Drawable* Drawable_newRect(Rectangle* r, Color* c)
{
    assert(NULL != r); assert(NULL != c);

    Drawable* d = malloc(sizeof(Drawable));
    d->sprite = NULL;
    d->rect = r;
    
    d->color = c;
    //Color* newC;
    //newC = Color_new(0.0f, 0.0f, 0.0f, 0.0f); //Well this is fugly. TODO: Should take 0 args. And Create Color_newFromRgba(f,f,f,f) instead.
    //newC->r = c->r;
    //newC->g = c->g;
    //newC->b = c->b;
    //newC->a = c->a;
    //d->color = newC;

    Drawable_invariant(d);
    return d;
}
*/


int Drawable_writeBatchData(Drawable* d, RenderBatch* batch, unsigned int start)
{
    if (NULL != ((Drawable*)d)->_writeBatchFunc)
        ((Drawable*)d)->_writeBatchFunc(d, batch, start);
}

void Drawable_delete(void* d)
{
    if (NULL != ((Drawable*)d)->_deleteFunc)
        ((Drawable*)d)->_deleteFunc(d);
}
