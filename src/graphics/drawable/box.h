/** @addtogroup smug_graphics
  * @{
  */

#ifndef GRAPHICS_DRAWABLE_BOX_H
#define GRAPHICS_DRAWABLE_BOX_H

#include <graphics/drawable/drawable.h>
#include <graphics/renderer/renderbatch.h>
#include <utils/vector_type.h>

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

#endif //GRAPHICS_DRAWABLE_BOX_H

/**@}*/
