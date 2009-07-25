#include "drawable.h"

#include "common/common.h"
#include <stdlib.h>

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
    Drawable_invariant(d);
    return d;
}

void Drawable_render(Drawable* d)
{
    Drawable_invariant(d);
    
    if (d->sprite)
    {
        Sprite_draw(d->sprite);
    }
    else
    {
        Graphics_fillRect(d->rect, d->color);
    }
}

void Drawable_delete(void* d)
{
    Drawable* dr = (Drawable*)d;
    Drawable_invariant(dr);
    
    if (dr->rect)
    {
        Rectangle_delete(dr->rect);
        Color_delete(dr->color);
    }
    
    // Intentionally NOT deleting Sprite, since sprites will probably be shared between objects.
    
    free((Drawable*)dr);
}
