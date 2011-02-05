/**
 * @file map.h
 * @brief Defines a map type and related functions.
 * @ingroup smug_utils
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_MAP_H
#define SMUG_UTILS_MAP_H

#include <common/common.h>
#include <utils/pair.h>
#include <utils/binarysearchtree.h>

/**
 * Map or associative array
 *
 * Map structure. Maps a key to a value.
 */
typedef struct Map
{
    struct BinarySearchTree base;        //< Inherit BinarySearchTree
    int (*compare)(void*, void*, void*); //< Comparison for keys
} Map;


/**
 * Iterator object for maps
 *
 * Struct use for iterating over a map.
 */
typedef struct MapIterator
{
    struct BinarySearchTreeIterator base; //< Inherit BinarySearchTreeIterator
} MapIterator;


/**
 * Create and return new Map
 *
 * @relates Map
 * @return The created map
 */
Map* Map_new(void);


/**
 * Initialize a map
 *
 * @relates Map
 * @param map The map to initialize
 */
void Map_init(Map* map);


/**
 * Delete a map
 *
 * @relates Map
 * @param map The map to delete
 */
void Map_delete(Map* map);


/**
 * Set the comparison function
 *
 * As with trees, set a comparison function that takes
 * a pointer to the map, and the two elements to be compared.
 * Should return -1, 0 or 1.
 *
 * @relates Map
 * @sa BinarySearchTree
 */
void Map_setCompare(Map* map, int (*compare)(void*, void*, void*));


/**
 * Set the value for a key in the map
 *
 * Sets a mapping from key to value
 *
 * @relates Map
 * @param map The map to insert into
 * @param key The key in the map
 * @param value The value in the map
 */
void Map_set(Map* map, void* key, void* value);


/**
 * Remove an entry from the map
 *
 * Removes an entry and return a Pair with the
 * key and value pointer.
 *
 * @relates Map
 * @param map The map to remove from
 * @param key The key to remove
 * @return A Pair with key and value removed
 */
Pair Map_remove(Map* map, void* key);


/**
 * Get the value mapped to a key
 *
 * Finds the value for a key and return it. Or
 * NULL if not found.
 *
 * @relates Map
 * @param map The map to search in
 * @param key The key to search for
 * @return The found value or NULL
 */
void* Map_get(Map* map, void* key);


/**
 * Create and return a new MapIterator
 *
 * @relates MapIterator
 * @return The created MapIterator
 */
MapIterator* MapIterator_new(void);


/**
 * Initialize an iterator
 *
 * @relates MapIterator
 * @param iter The iterator to init
 */
void MapIterator_init(MapIterator* iter);


/**
 * Delete a MapIterator
 *
 * @relates MapIterator
 * @param iter The MapIterator to remove.
 */
void MapIterator_delete(MapIterator* iter);


/**
 * Reset the MapIterator as forward iterator
 *
 * @relates MapIterator
 * @param map The Map to reset for.
 * @param iter The MapIterator to reset.
 */
void MapIterator_reset(Map* map, MapIterator* iter);


/**
 * Step the MapIterator forward
 *
 * @relates MapIterator
 * @param iter The MapIterator to step
 */
void MapIterator_step(MapIterator* iter);


/**
 * Check validity of a MapIterator
 *
 * @relates MapIterator
 * @param iter The MapIterator to check
 * @return TRUE or FALSE depending on validity
 */
BOOL MapIterator_valid(MapIterator* iter);


/**
 * Get the current value of the iterator
 *
 * @relates MapIterator
 * @param iter The MapIterator to get for
 * @return The value or NULL if the iterator is invalid
 */
void* MapIterator_get(MapIterator* iter);


#endif // SMUG_UTILS_MAP_H

/**@}*/
