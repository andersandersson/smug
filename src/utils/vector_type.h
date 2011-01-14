/** @file vector_type.h
  * @brief Defines the Vector type.
  * @ingroup smug_utils
  */

/** @addtogroup smug_utils
  * @{
  */

#ifndef SMUG_UTILS_VECTOR_TYPE_H
#define SMUG_UTILS_VECTOR_TYPE_H

/**
 * Simple 2 dimensional vector.
 */
typedef struct Vector2d
{
    int size;    //!< The dimension of the vector
    float d[2];
} Vector;

#endif /* SMUG_UTILS_VECTOR_TYPE_H */

/**@}*/
