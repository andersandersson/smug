/** @file shapes.h
  * @brief
  * @ingroup smug_physics
  */

/** @addtogroup smug_physics
  * @{
  */

#ifndef SMUG_PHYSICS_SHAPES_H
#define SMUG_PHYSICS_SHAPES_H

#include "utils/rectangle.h"
#include "utils/point.h"
#include "utils/vector.h"

/** Describes the type of a Shape */
typedef unsigned int SHAPE_TYPE;

#define SHAPE_UNKNOWN   0x00
#define SHAPE_RECTANGLE 0x01


/** Container object for shapes
 *
 * A container object for different shapes. This struct will
 * hide the different shape implementations from the user.
 */
typedef struct Shape
{
    /** The type of the shape, e.g. SHAPE_RECTANGLE */
    SHAPE_TYPE type;
    /** Pointer to the real data for the shape */
    void* data;
} Shape;



/** Create a new Shape
 *
 * This will create a new empty shape object. In most cases this
 * is not very useful, use the Shape_createFrom* instead.
 *
 * @relatesalso Shape
 * @return Shape
 */
Shape* Shape_new(void);


/** Free memory used by a Shape
 * @relatesalso Shape
 */
void Shape_delete(Shape* shape);


/** Create a new Shape based on a Rectangle
 *
 * This function takes a Rectangle object and creates a new
 * Shape object from it.
 *
 * @param rect The Rectangle to create a shape from
 * @relatesalso Shape
 * @return Shape
 */
Shape* Shape_createFromRectangle(Rectangle rect);

#endif // SMUG_PHYSICS_SHAPES_H

/**@}*/
