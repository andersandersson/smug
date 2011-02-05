/** @file shape_type.h
 * @brief
 * @ingroup smug_utils
 */

/** @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_SHAPES_TYPE_H
#define SMUG_UTILS_SHAPES_TYPE_H

struct _Shape;
typedef struct _Shape Shape;

/** Describes the type of a Shape */
typedef unsigned int SHAPE_TYPE;

#define SHAPE_UNKNOWN       0x00
#define SHAPE_RECTANGLE     0x01
#define SHAPE_UNFINISHED    0x02
#define SHAPE_MULTIPOINT    0x03

/** Container object for shapes
 *
 * A container object for different shapes. This struct will
 * hide the different shape implementations from the user.
 */
typedef struct _Shape
{
    /** The type of the shape, e.g. SHAPE_RECTANGLE */
    SHAPE_TYPE type;
    /** Pointer to the real data for the shape */
    void* data;
} _Shape;

#endif // SMUG_UTILS_SHAPES_TYPE_H

/**@}*/
