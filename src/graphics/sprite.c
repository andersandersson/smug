#include <smugstd.h>

#include <common/common.h>
#include <utils/vector.h>
#include <utils/rectangle.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/texture_type.h>
#include <graphics/texture/texture_internal.h>

#include <graphics/sprite.h>

Sprite* Sprite_newFromPixelCoords(Texture* texture, Rectangle rect, Vector offset)
{
    smug_assert(texture != NULL);
    Sprite* s = (Sprite*)malloc(sizeof(Sprite));
    s->texture = texture;
    s->rect = rect;
    s->offset = offset;
    return s;
}

Sprite* Sprite_newFromTextureCoords(Texture* texture, Rectangle rect, Vector offset)
{
    smug_assert(texture != NULL);
    Sprite* s = (Sprite*)malloc(sizeof(Sprite));
    s->texture = texture;
    s->rect = Rectangle_multiplyVector(rect, Vector_create2d(texture->width, texture->height));
    s->offset = Vector_multiplyVector(offset, Vector_create2d(texture->width, texture->height));
    return s;
}

Texture* Sprite_getTexture(Sprite* self)
{
    smug_assert(self != NULL);
    return self->texture;
}

unsigned int Sprite_getTextureId(Sprite* self)
{
    smug_assert(self != NULL);
    return Texture_getId(self->texture);
}

void Sprite_delete(void* sprite)
{
    if (NULL != sprite)
    {
        free(sprite);
    }
}
