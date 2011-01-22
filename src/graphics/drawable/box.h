/** @addtogroup smug_graphics
  * @{
  */

#ifndef GRAPHICS_DRAWABLE_BOX_H
#define GRAPHICS_DRAWABLE_BOX_H

struct DrawableShape;
#include <utils/vector_type.h>

/**
 * Creates new box drawable
 * @relatesalso Drawable
 * @return Drawable
 */
struct DrawableShape* DrawableShape_newBox(void);

/**
 * Creates new box drawable from a rectangle
 * @relatesalso Drawable
 * @param rect Rectangle
 * @return Drawable
 */
struct DrawableShape* DrawableShape_newBoxFromSize(Vector size);

#endif //GRAPHICS_DRAWABLE_BOX_H

/**@}*/
