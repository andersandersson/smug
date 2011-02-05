/**
 * @file texture.h
 * @brief Defines the Texture type functions.
 * @ingroup smug_graphics
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_TEXTURE_TEXTURE_H
#define SMUG_GRAPHICS_TEXTURE_TEXTURE_H

#include <common/common.h>
#include <graphics/image/image.h>
#include <graphics/texture/texture_type.h>

/**
 * Creates a new texture with the specified size.
 * @relates _Texture
 */
Texture* Texture_new(unsigned int width, unsigned int height);

/**
 * Creates a new texture from a loaded image.
 * @relates _Texture
 */
Texture* Texture_newFromImage(Image* image);

/**
 * @relates _Texture
 */
void Texture_release(Texture* texture);

/**
 * @relates _Texture
 */
void Texture_reload(Texture* texture);

/**
 * @relates _Texture
 */
void Texture_delete(void* texture);

/**
 * @relates _Texture
 */
unsigned int Texture_getId(Texture* self);

/**
 * @relates _Texture
 */
unsigned int Texture_getWidth(Texture* self);

/**
 * @relates _Texture
 */
unsigned int Texture_getHeight(Texture* self);

#endif // SMUG_GRAPHICS_TEXTURE_TEXTURE_H

/**@}*/
