/** @file sprite.h
  * @brief Defines the Sprite type.
  * @ingroup smug_graphics
  */

/** @addtogroup smug_graphics
  * @{
  */

#ifndef SMUG_GRAPHICS_SPRITE_H
#define SMUG_GRAPHICS_SPRITE_H

#include "graphics/texture/texture.h"
#include "utils/rectangle.h"
#include "utils/vector.h"

/** 
 * Holds a texture and a rectangular area on it.
 * Also supports an offset to that area.
 */
typedef struct Sprite
{
    Rectangle rect; 
    Texture* texture;
    Vector offset;
} Sprite;

/*
 * Create new sprite from pixel coordinates.
 * @param texture a pointer to a valid texture
 * @param rect a rectangle on the texture
 * @param offset offset of the rect
 */
Sprite* Sprite_newFromPixelCoords(Texture* texture, Rectangle rect, Vector offset);

/*
 * Create new sprite from texture coordinates
 * @param texture a pointer to a valid texture
 * @param rect a rectangle on the texture
 * @param offset offset of the rect 
 */
Sprite* Sprite_newFromTextureCoords(Texture* texture, Rectangle rect, Vector offset);

/*
 * Delete sprite
 * @param sprite a pointer to a sprite
 */
void Sprite_delete(void* sprite);

#endif // SMUG_GRAPHICS_SPRITE_H

/**@}*/
