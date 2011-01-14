#ifndef SMUG_UTILS_MAP_H
#define SMUG_UTILS_MAP_H

#include "common/common.h"

typedef struct MapNode
{
    struct MapNode* parent;
    struct MapNode* left;
    struct MapNode* right;
    void* key;
    void* value;
    int height;
} MapNode;

typedef struct Map
{
    struct MapNode* root;
    int (*compareKeys)(void*, void*);
    int (*compareValues)(void*, void*);
} Map;

typedef struct MapIterator 
{
    struct Map* map;
    struct MapNode* current;  
} MapIterator;

MapNode* MapNode_new(void);
void MapNode_delete(MapNode* node);
void MapNode_deleteRecursive(MapNode* node);
void MapNode_copy(MapNode* left, MapNode* right);

Map* Map_new(void);
void Map_delete(Map* map);
void Map_set(Map* map, void* key, void* value);
void* Map_get(Map* map, void* key);
MapNode* Map_findByKey(Map* map, void* key);
MapNode* Map_findByValue(Map* map, void* value);
void Map_remove(Map* map, void* key);

void Map_print(Map* map);


MapIterator* MapIterator_new(Map* map);
void MapIterator_delete(MapIterator* iter);
void MapIterator_reset(MapIterator* iter);
void MapIterator_step(MapIterator* iter);
MapNode* MapIterator_get(MapIterator* iter);


#endif // SMUG_UTILS_MAP_H
