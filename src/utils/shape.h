/** @file shapes.h
  * @brief
  * @ingroup smug_physics
  */

/** @addtogroup smug_physics
  * @{
  */

#ifndef SMUG_UTILS_SHAPES_H
#define SMUG_UTILS_SHAPES_H

#include <common/common.h>
#include <utils/rectangle_type.h>
#include <utils/shape_type.h>

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
Shape* Shape_newFromRectangle(Rectangle rect);
Rectangle Shape_getAsRectangle(Shape* self);

Shape* Shape_newMultipoint(void);
Shape* Shape_addPoint(Shape* self, Vector point);
Shape* Shape_finishMultipoint(Shape* self);

Vector Shape_getFirstPoint(Shape* self);
BOOL Shape_morePoints(Shape* self);
Vector Shape_getNextPoint(Shape* self);
int Shape_getNrPoints(Shape* self);

SHAPE_TYPE Shape_getType(Shape* self);
void Shape_moveByVector(Shape* self, Vector v);
void Shape_scale(Shape* self, float scale);
void Shape_scaleX(Shape* self, float scale);
void Shape_scaleY(Shape* self, float scale);
void Shape_scaleXY(Shape* self, Vector scales);

#endif // SMUG_UTILS_SHAPES_H

/**@}*/
