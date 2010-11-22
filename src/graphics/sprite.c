#include "sprite.h"
#include <stdlib.h>

Sprite* Sprite_newFromPixelCoords(Texture* texture, Rectangle rect, Vector offset)
{
    assert(texture != NULL);
    Sprite* s = (Sprite*)malloc(sizeof(Sprite));
    s->texture = texture;
    s->rect = rect;
    s->offset = offset;
    return s;
}

Sprite* Sprite_newFromTextureCoords(Texture* texture, Rectangle rect, Vector offset)
{
    assert(texture != NULL);
    Sprite* s = (Sprite*)malloc(sizeof(Sprite));
    s->texture = texture;
    s->rect = Rectangle_multiplyVector(rect, Vector_create2d(texture->width, texture->height));
    s->offset = Vector_multiplyVector(offset, Vector_create2d(texture->width, texture->height));
    return s;
}

void Sprite_delete(void* sprite)
{
    if (NULL != sprite)
    {
        free(sprite);
    }
}
