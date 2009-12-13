/** @addtogroup smug_graphics
  * @{
  */

#ifndef GRAPHICS_DRAWABLE_BOX_H
#define GRAPHICS_DRAWABLE_BOX_H

#include "drawable.h"
#include "graphics/renderer/renderbatch.h"
#include "utils/rectangle.h"
#include "utils/vector.h"

/**
 * Creates new box drawable
 * @relatesalso Drawable
 * @return Drawable
 */
Drawable* Drawable_newBox();

/**
 * Creates new box drawable from a rectangle
 * @relatesalso Drawable
 * @param rect Rectangle
 * @return Drawable
 */
Drawable* Drawable_newBoxFromRectangle(Rectangle rect);

#endif //GRAPHICS_DRAWABLE_BOX_H

/**@}*/
