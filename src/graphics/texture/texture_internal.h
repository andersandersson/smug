/**
 * @file texture_internal.h
 * @brief Defines the Texture type.
 * This will be included from files working directly with the internals of the Texture type.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_TEXTURE_TEXTURE_INTERNAL_H
#define SMUG_GRAPHICS_TEXTURE_TEXTURE_INTERNAL_H

/**
 * A Texture is a piece of graphic used to create Sprites.
 */
typedef struct _Texture
{
    Image* image;
    unsigned int texid;
    unsigned int width;
    unsigned int height;
    float px;
    float py;
    BOOL loaded;
} _Texture;

#endif /* SMUG_GRAPHICS_TEXTURE_TEXTURE_INTERNAL_H */

/**@}*/
