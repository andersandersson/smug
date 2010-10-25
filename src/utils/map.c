#include "map.h"

#include "common/log.h"
#include "common/common.h"

#include <stdlib.h>

static void Map_addLast(Map* map, MapNode* node);

MapNode* MapNode_new(void)
{
    MapNode* node = malloc(sizeof(MapNode));

    node->next = NULL;
    node->key = NULL;
    node->value = NULL;

    return node;
}

void MapNode_delete(MapNode* node)
{
    free(node);
}

Map* Map_new(void)
{
    Map* map = malloc(sizeof(Map));

    map->compareKeys = NULL;
    map->compareValues = NULL;
    map->first = NULL;

    return map;
}

void Map_delete(Map* map)
{
    MapNode* node;
    MapNode* prev_node;

    node = map->first;

    if(node != NULL)
        {
            while(node->next != NULL)
                {
                    prev_node = node;
                    node = node->next;
                    
                    MapNode_delete(prev_node);
                }
            
            
            MapNode_delete(node);
        }

    free(map);
}

void Map_addLast(Map* map, MapNode* node)
{
    MapNode* _node = map->first;

    if(_node == NULL)
        {
            map->first = node;
        }
    else
        {
            while(_node->next != NULL)
                {
                    _node = _node->next;
                }

            _node->next = node;
        }    
}

void Map_set(Map* map, void* key, void* value)
{
    MapNode* node = Map_findByKey(map, key);

    if(NULL == node)
        {
            node = MapNode_new();
            node->key = key;
            node->value = value;

            Map_addLast(map, node);
        }
    else
        {
            node->value = value;
        }
}

void* Map_get(Map* map, void* key)
{
    MapNode* node = Map_findByKey(map, key);

    if(node != NULL)
        {
            return node->value;
        }

    return NULL;
}

MapNode* Map_findByKey(Map* map, void* key)
{
    MapNode* node;

    for(node = map->first; node != NULL; node = node->next)
        {
            if(NULL == map->compareKeys)
                {
                    if(key == node->key)
                        {
                            return node;
                        }
                }
            else
                {
                    if(TRUE == map->compareKeys(key, node->key))
                        {
                            return node;
                        }            
                }
        }

    return NULL;
}

MapNode* Map_findByValue(Map* map, void* value)
{
    MapNode* node;

    for(node = map->first; node != NULL; node = node->next)
        {
            if(NULL == map->compareValues)
                {
                    if(value == node->value)
                        {
                            return node;
                        }
                }
            else
                {
                    if(TRUE == map->compareValues(value, node->value))
                        {
                            return node;
                        }            
                }
        }

    return NULL;
}

void Map_remove(Map* map, void* key)
{
}
