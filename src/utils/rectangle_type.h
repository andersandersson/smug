/** @file rectangle_type.h
  * @brief Defines a rectangle type.
  */

#ifndef SMUG_UTILS_RECTANGLE_TYPE_H
#define SMUG_UTILS_RECTANGLE_TYPE_H

#include <utils/vector_type.h>
#include <utils/point_type.h>

/** A simple rectangle type.
  *
  */
typedef struct Rectangle
{
    Point pos;
    Vector size;
} Rectangle;

#endif // SMUG_UTILS_RECTANGLE_TYPE_H
