/** @addtogroup smug_graphics
  * @{
  */

#ifndef GRAPHICS_DRAWABLE_BOX_H
#define GRAPHICS_DRAWABLE_BOX_H

#include <utils/vector_type.h>
#include <graphics/drawable/drawable_type.h>
#include <graphics/sprite.h>

/**
 * Creates new box drawable
 * @relatesalso Drawable
 * @return Drawable
 */
Drawable* Drawable_newBox(void);

/**
 * Creates new box drawable from a rectangle
 * @relatesalso Drawable
 * @param rect Rectangle
 * @return Drawable
 */
Drawable* Drawable_newBoxFromSize(Vector size);

Drawable* Drawable_newFromSprite(Sprite* sprite);

#endif //GRAPHICS_DRAWABLE_BOX_H

/**@}*/
