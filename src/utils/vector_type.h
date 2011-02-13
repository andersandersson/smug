/**
 * @file vector_type.h
 * @brief Defines the Vector type.
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_VECTOR_TYPE_H
#define SMUG_UTILS_VECTOR_TYPE_H

/**
 * Simple 2 dimensional vector.
 */
typedef struct Vector
{
    int size;    //!< The dimension of the vector
    float d[2];
} Vector;

#endif /* SMUG_UTILS_VECTOR_TYPE_H */

/**@}*/
