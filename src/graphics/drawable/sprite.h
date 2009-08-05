#ifndef SMUG_GRAPHICS_SPRITE_H
#define SMUG_GRAPHCIS_SPRITE_H

/** Placeholder struct for Sprite type.
  */
typedef struct Sprite
{
    int placeholder;
} Sprite;

Sprite* Sprite_new();

void Sprite_delete(Sprite* sprite);

void Sprite_draw(Sprite* sprite, float x, float y);

#endif // SMUG_GRAPHICS_SPRITE_H
