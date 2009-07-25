#ifndef SPRITE_H
#define SPRITE_H

/** Placeholder struct for Sprite type.
  */
typedef struct Sprite
{
	int placeholder;
} Sprite;

Sprite* Sprite_new();

void Sprite_delete(Sprite* sprite);

void Sprite_draw(Sprite* sprite);

#endif // SPRITE_H
