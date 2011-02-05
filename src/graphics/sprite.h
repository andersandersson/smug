/**
 * @file sprite.h
 * @brief Defines the Sprite type and related functions.
 * @ingroup smug_graphics
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_SPRITE_H
#define SMUG_GRAPHICS_SPRITE_H

#include <common/common.h>
#include <utils/rectangle_type.h>
#include <utils/vector_type.h>
#include <graphics/texture/texture.h>

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

/**
 * Create new sprite from pixel coordinates.
 * @param texture a pointer to a valid texture
 * @param rect a rectangle on the texture
 * @param offset offset of the rect
 * @relates Sprite
 */
Sprite* Sprite_newFromPixelCoords(Texture* texture, Rectangle rect, Vector offset);

/**
 * Create new sprite from texture coordinates
 * @param texture A pointer to a valid texture
 * @param rect A rectangle on the texture
 * @param offset Offset of the rect
 * @relates Sprite
 */
Sprite* Sprite_newFromTextureCoords(Texture* texture, Rectangle rect, Vector offset);

/**
 * Return the texture the sprite draws its graphic from.
 * @relates Sprite
 */
Texture* Sprite_getTexture(Sprite* self);

/**
 * Return the ID of the texture the sprite draws its graphic from.
 * @relates Sprite
 */
unsigned int Sprite_getTextureId(Sprite* self);

/**
 * Delete sprite
 * @param sprite a pointer to a sprite
 * @relates Sprite
 */
void Sprite_delete(void* sprite);

#endif // SMUG_GRAPHICS_SPRITE_H

/**@}*/
