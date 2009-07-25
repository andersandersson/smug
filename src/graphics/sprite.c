#include "sprite.h"
#include <stdlib.h>

Sprite* Sprite_new()
{
    return (Sprite*)malloc(sizeof(Sprite));
}

void Sprite_delete(Sprite* sprite)
{
    if (NULL != sprite)
    {
        free(sprite);
    }
}

void Sprite_draw(Sprite* sprite)
{

}
