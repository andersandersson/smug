#ifndef SMUG_GRAPHICS_SPRITE_H
#define SMUG_GRAPHCIS_SPRITE_H

#include "drawable.h"
#include "graphics/texture/texture.h"
#include "utils/rectangle.h"
#include "utils/vector.h"

typedef struct Sprite
{
    Drawable base;
    Rectangle rect; 
    Texture* texture;
    Color color;
    Vector offset;
} Sprite;

Sprite* Sprite_newFromPixelCoords(Texture* texture, Rectangle rect, Vector offest);
Sprite* Sprite_newFromTextureCoords(Texture* texture, Rectangle rect, Vector offset);
void Sprite_delete(void* sprite);

int Sprite_writeBatchData(Drawable* box, RenderBatch* batch, unsigned int start);

#endif // SMUG_GRAPHICS_SPRITE_H
