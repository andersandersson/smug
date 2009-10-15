#ifndef SMUG_UTILS_MAP_H
#define SMUG_UTILS_MAP_H

#include "common/common.h"

typedef struct MapNode
{
    struct MapNode* next;
    void* key;
    void* value;
} MapNode;

typedef struct Map
{
    struct MapNode* first;
    BOOL (*compareKeys)(void*, void*);
    BOOL (*compareValues)(void*, void*);
} Map;


MapNode* MapNode_new();
void MapNode_delete();

Map* Map_new();
void Map_delete(Map* map);
void Map_set(Map* map, void* key, void* value);
void* Map_get(Map* map, void* key);
MapNode* Map_findByKey(Map* map, void* key);
MapNode* Map_findByValue(Map* map, void* value);
void Map_remove(Map* map, void* key);

#endif // SMUG_UTILS_MAP_H
