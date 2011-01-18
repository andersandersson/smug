#ifndef SMUG_UTILS_MAP_H
#define SMUG_UTILS_MAP_H

#include <common/common.h>
#include <utils/pair.h>

// Forward declarations
struct BinarySearchTree;
struct BinarySearchTreeIterator;

/** Map or associative array 
  *
  * Map structure. Maps a key to a value.
  */
typedef struct Map
{
  struct BinarySearchTree* tree;       //< Current implementation, tree
  int (*compare)(void*, void*, void*); //< Comparison for keys
} Map;


/** Iterator object for maps
  * 
  * Struct use for iterating over a map.
  */
typedef struct MapIterator 
{
  struct BinarySearchTreeIterator* iterator; //< Current implementation, teree
} MapIterator;


/** Create and return new Map
  * 
  * @relatesalso Map
  * @return The created map
  */
Map* Map_new(void);


/** Delete a map
  *
  * @relatesalso Map
  * @param map The map to delete
  */
void Map_delete(Map* map);


/** Set the comparison function
  *
  * As with trees, set a comparison function that takes
  * a pointer to the map, and the two elements to be compared.
  * Should return -1, 0 or 1.
  *
  * @relatesalso Map
  * @sa BinarySearchTree
  */
void Map_setCompare(Map* map, int (*compare)(void*, void*, void*));


/** Set the value for a key in the map
  *
  * Sets a mapping from key to value
  *
  * @relatesalso Map
  * @param map The map to insert into
  * @param key The key in the map
  * @param value The value in the map
  */
void Map_set(Map* map, void* key, void* value);


/** Remove an entry from the map
  * 
  * Removes an entry and return a Pair with the
  * key and value pointer.
  *
  * @relatesalso Map
  * @param map The map to remove from
  * @param key The key to remove
  * @return A Pair with key and value removed
  */
Pair Map_remove(Map* map, void* key);


/** Get the value mapped to a key
  *
  * Finds the value for a key and return it. Or
  * NULL if not found.
  *
  * @relatesalso Map
  * @param map The map to search in
  * @param key The key to search for
  * @return The found value or NULL
  */
void* Map_get(Map* map, void* key);


/** Create and return a new MapIterator
  *
  * @relatesalso MapIterator
  * @return The created MapIterator
  */
MapIterator* MapIterator_new(void);


/** Delete a MapIterator
  *
  * @relatesalso MapIterator
  * @param iter The MapIterator to remove.
  */
void MapIterator_delete(MapIterator* iter);


/** Reset the MapIterator as forward iterator 
  *
  * @relatesalso MapIterator
  * @param map The Map to reset for.
  * @param iter The MapIterator to reset.
  */
void MapIterator_reset(Map* map, MapIterator* iter);


/** Step the MapIterator forward
  *
  * @relatesalso MapIterator
  * @param iter The MapIterator to step
  */
void MapIterator_step(MapIterator* iter);


/** Check validity of a MapIterator
  *
  * @relatesalso MapIterator
  * @param iter The MapIterator to check
  * @return TRUE or FALSE depending on validity
  */
BOOL MapIterator_valid(MapIterator* iter);


/** Get the current value of the iterator
  *
  * @relatesalso MapIterator
  * @param iter The MapIterator to get for
  * @return The value or NULL if the iterator is invalid
  */
void* MapIterator_get(MapIterator* iter);


#endif // SMUG_UTILS_MAP_H
