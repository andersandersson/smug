#ifndef SMUG_GRAPHICS_SPRITE_H
#define SMUG_GRAPHICS_SPRITE_H

#include "graphics/texture/texture.h"
#include "utils/rectangle.h"
#include "utils/vector.h"

typedef struct Sprite
{
    Rectangle rect; 
    Texture* texture;
    Vector offset;
} Sprite;

Sprite* Sprite_newFromPixelCoords(Texture* texture, Rectangle rect, Vector offest);
Sprite* Sprite_newFromTextureCoords(Texture* texture, Rectangle rect, Vector offset);
void Sprite_delete(void* sprite);


#endif // SMUG_GRAPHICS_SPRITE_H
