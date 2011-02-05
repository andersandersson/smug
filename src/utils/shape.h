/**
 * @file shape.h
 * @brief Define the functions related to the Shape type.
 * @ingroup smug_utils
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_SHAPES_H
#define SMUG_UTILS_SHAPES_H

#include <common/common.h>
#include <utils/rectangle_type.h>
#include <utils/shape_type.h>

/**
 * Create a new Shape
 *
 * This will create a new empty shape object. In most cases this
 * is not very useful, use the Shape_createFrom* instead.
 *
 * @relates _Shape
 * @return Shape
 */
Shape* Shape_new(void);


/**
 * Free memory used by a Shape
 * @relates _Shape
 */
void Shape_delete(Shape* shape);


/**
 * Create a new Shape based on a Rectangle
 *
 * This function takes a Rectangle object and creates a new
 * Shape object from it.
 *
 * @param rect The Rectangle to create a shape from
 * @relates _Shape
 * @return Shape
 */
Shape* Shape_newFromRectangle(Rectangle rect);

/**
 * Get the shape as a rectangle.
 * @relates _Shape
 * @pre The shape must be of type SHAPE_RECTANGLE
 */
Rectangle Shape_getAsRectangle(Shape* self);

/**
 * Create a new multipoint shape. Its type will be SHAPE_UNFINISHED until Shape_finishMultipoint is
 * called on it.
 * @relates _Shape
 */
Shape* Shape_newMultipoint(void);

/**
 * Add a point to an unfinished multipoint.
 * @relates _Shape
 * @pre The type of the shape must be SHAPE_UNFINISHED.
 */
Shape* Shape_addPoint(Shape* self, Vector point);

/**
 * Finish an unfinished multipoint. Its type will be set to SHAPE_MULTIPOINT.
 * @relates _Shape
 * @pre The type of the shape must be SHAPE_UNFINISHED.
 */
Shape* Shape_finishMultipoint(Shape* self);

/**
 * Get thefirst point in a multipoint shape.
 * @relates _Shape
 * @pre The type of the shape must be SHAPE_MULTIPOINT.
 */
Vector Shape_getFirstPoint(Shape* self);

/**
 * Check if a multipoint shape has more points.
 * @relates _Shape
 * @pre The type of the shape must be SHAPE_MULTIPOINT.
 */
BOOL Shape_morePoints(Shape* self);

/**
 * Get the next point in a multipoint shape.
 * @relates _Shape
 * @pre The type of the shape must be SHAPE_MULTIPOINT.
 */
Vector Shape_getNextPoint(Shape* self);

/**
 * Get the number of points in a multipoint shape.
 * @relates _Shape
 * @pre The type of the shape must be SHAPE_MULTIPOINT.
 */
int Shape_getNrPoints(Shape* self);

/**
 * Get the type of a shape.
 * @relates _Shape
 */
SHAPE_TYPE Shape_getType(Shape* self);

/**
 * Move all points in a shape by a vector.
 * @relates _Shape
 */
void Shape_moveByVector(Shape* self, Vector v);

/**
 * Scale all points in a shape by a number.
 * @relates _Shape
 */
void Shape_scale(Shape* self, float scale);

/**
 * Scale all points in a shape by a number (along the X axis).
 * @relates _Shape
 */
void Shape_scaleX(Shape* self, float scale);

/**
 * Scale all points in a shape by a number (along the Y axis).
 * @relates _Shape
 */
void Shape_scaleY(Shape* self, float scale);

/**
 * Scale all points in a shape by a number (separate numbers along the X and Y axis).
 * @relates _Shape
 */
void Shape_scaleXY(Shape* self, Vector scales);

#endif // SMUG_UTILS_SHAPES_H

/**@}*/
