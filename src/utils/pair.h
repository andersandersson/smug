/**
 * @file pair.h
 * @brief Defines a pair type nad related functions.
 * @ingroup smug_utils
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_PAIR_H
#define SMUG_UTILS_PAIR_H

/**
 * Simple Pair struct
 *
 * Not much to say. It's a Pair.
 */
typedef struct Pair
{
  void* left;  //<
  void* right; //<
} Pair;


/**
 * Create a new Pair
 *
 * @relates Pair
 */
Pair* Pair_new(void);


/**
 * Delete a Pair
 *
 * @relates Pair
 * @param pair The Pair to be deleted
 */
void Pair_delete(Pair* pair);

#endif // SMUG_UTILS_PAIR_H

/**@}*/
