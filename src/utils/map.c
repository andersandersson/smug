#include <smugstd.h>
#include <utils/map.h>
#include <utils/pair.h>
#include <utils/binarysearchtree.h>

static int _pair_compare(void* map, void* left, void* right)
{
  Pair* _left = (Pair*) left;
  Pair* _right = (Pair*) right;
  Map* _map = (BinarySearchTree*) map;

  return _map->compare(_left->left, _right->left);
}

static int _default_compare(void* left, void* right)
{
  if(left < right)
    {
      return -1;
    }
  else if(left > right)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

Map* Map_new(void)
{
  Map* map = malloc(sizeof(Map));

  map->tree = BinarySearchTree_new();
  map->compare = _default_compare;

  BinarySearchTree_setCompare(map->tree, map, _pair_compare);

  return map;
}

void Map_delete(Map* map)
{
  BinarySearchTree_delete(map->tree);

  free(map);
}


void Map_setCompare(Map* map, int (*compare)(void*, void*))
{
  map->compare = compare;
  BinarySearchTree_setCompare(map->tree, map, _pair_compare);
}

void Map_set(Map* map, void* key, void* value)
{
  Pair* pair = Pair_new();

  pair->left = key;
  pair->right = value;

  BinarySearchTree_insert(map->tree, pair);
}

Pair Map_remove(Map* map, void* key)
{
  Pair search;

  search.left = key;

  Pair* found = BinarySearchTree_remove(map->tree, &search);

  Pair pair;

  if(NULL == found)
    {
      pair.left = NULL;
      pair.right = NULL;
    }
  else
    {
      pair.left = found->left;
      pair.right = found->right;
    }

  Pair_delete(found);

  return pair;
}

void* Map_get(Map* map, void* key)
{
  Pair search;
  search.left = key;

  Pair* pair = BinarySearchTree_find(map->tree, &search);

  if(NULL == pair)
    {
      return NULL;
    }

  return pair->right;
}

void Map_print(Map* map)
{
  BinarySearchTree_print(map->tree);
}




MapIterator* MapIterator_new(void)
{
  MapIterator* iter = malloc(sizeof(MapIterator));

  iter->iterator = BinarySearchTreeIterator_new();

  return iter;
}


void MapIterator_delete(MapIterator* iter)
{
  if(NULL == iter)
    {
      return;
    }

  BinarySearchTreeIterator_delete(iter->iterator);
  
  free(iter);
}


void MapIterator_reset(Map* map, MapIterator* iter)
{
  BinarySearchTreeIterator_reset(map->tree, iter->iterator);
}


void MapIterator_step(MapIterator* iter)
{
  BinarySearchTreeIterator_step(iter->iterator);
}


BOOL MapIterator_valid(MapIterator* iter)
{
  return BinarySearchTreeIterator_valid(iter->iterator);
}


void* MapIterator_get(MapIterator* iter)
{
  return BinarySearchTreeIterator_get(iter->iterator);
}


