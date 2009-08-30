#ifndef SMUG_GRAPHICS_SPRITE_H
#define SMUG_GRAPHCIS_SPRITE_H

#include "drawable.h"

typedef struct Sprite
{
    Drawable base;
    int textureId;
} Sprite;

Sprite* Sprite_new();
void Sprite_delete(Sprite* sprite);


#endif // SMUG_GRAPHICS_SPRITE_H
