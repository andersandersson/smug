/**
 * @file box.h
 * @brief Defines functions to create Drawables that are boxes.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef GRAPHICS_DRAWABLE_BOX_H
#define GRAPHICS_DRAWABLE_BOX_H

#include <utils/vector_type.h>
#include <graphics/drawable/drawable_type.h>
#include <graphics/sprite.h>

/**
 * Creates new box drawable. It has zero size, no sprite, and opaque, black color. Color is turned
 * on.
 * @relates Drawable
 */
Drawable* Drawable_newBox(void);

/**
 * Creates new box drawable from a size. It has the specified size, no sprite, and opaque,
 * black color. Color is turned on.
 * @param size A vector specifying the size of the box.
 * @return Drawable
 * @relates Drawable
 */
Drawable* Drawable_newBoxFromSize(Vector size);

/**
 * Creates new box drawable from a sprite. It has the specified sprite's size and opaque, black
 * color. Color is turned off.
 * @relates Drawable
 * @param rect Rectangle
 * @return Drawable
 */
Drawable* Drawable_newFromSprite(Sprite* sprite);

#endif //GRAPHICS_DRAWABLE_BOX_H

/**@}*/
