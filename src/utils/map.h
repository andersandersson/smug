#ifndef SMUG_UTILS_MAP_H
#define SMUG_UTILS_MAP_H

#include <common/common.h>
#include <utils/pair.h>

typedef struct Map
{
    struct BinarySearchTree* tree;
  int (*compare)(void*, void*, void*);
} Map;


typedef struct MapIterator 
{
    struct BinarySearchTreeIterator* iterator;
} MapIterator;


Map* Map_new(void);
void Map_delete(Map* set);
void Map_setCompare(Map* set, int (*compare)(void*, void*, void*));
void Map_set(Map* set, void* key, void* value);
Pair Map_remove(Map* set, void* key);
void* Map_get(Map* set, void* key);
void Map_print(Map* set);


MapIterator* MapIterator_new(void);
void MapIterator_delete(MapIterator* iter);
void MapIterator_reset(Map* set, MapIterator* iter);
void MapIterator_step(MapIterator* iter);
BOOL MapIterator_valid(MapIterator* iter);
void* MapIterator_get(MapIterator* iter);


#endif // SMUG_UTILS_MAP_H
