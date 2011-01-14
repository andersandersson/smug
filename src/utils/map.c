#include "map.h"

#include "common/log.h"
#include "common/common.h"

#include <stdlib.h>

MapNode* MapNode_new(void)
{
    MapNode* node = malloc(sizeof(MapNode));

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = NULL;
    node->value = NULL;
    node->height = 0;

    return node;
}


void MapNode_deleteRecursive(MapNode* node)
{
  if(NULL == node)
    {
      return;
    }

  if(NULL != node->left)
    {
      MapNode_delete(node->left);
    }

  if(NULL != node->right)
    {
      MapNode_delete(node->right);
    }

  free(node);
}


void MapNode_delete(MapNode* node)
{
  if(NULL == node)
    {
      return;
    }

  free(node);
}


void MapNode_copy(MapNode* self, MapNode* other)
{
    self->left = other->left;
    self->right = other->right;
    self->parent = other->parent;
    self->key = other->key;
    self->value = other->value;
    self->height = other->height;
}

int _Map_defaultCompare(void* left, void* right)
{
  if(left == right)
    {
      return 0;
    }
  else if(left < right)
    {
      return -1;
    }
  else if(left > right)
    {
      return 1;
    }
}

Map* Map_new(void)
{
    Map* map = malloc(sizeof(Map));

    map->compareKeys = _Map_defaultCompare;
    map->compareValues = _Map_defaultCompare;
    map->root = NULL;

    return map;
}

void Map_delete(Map* map)
{
    MapNode_deleteRecursive(map->root);

    free(map);
}

void _Map_insert(Map* map, MapNode* node, void* key, void* value, int height)
{
  int side = map->compareKeys(key, node->key);
  int new_height = node->height;

  if(0 == side)
    {
    }
  else if(-1 == side)
    {
      if(NULL == node->left)
	{
	  if(NULL == node->right)
	    {
	      node->height = 1;
	    }

	  node->left = MapNode_new();
	  node->left->key = key;
	  node->left->value = value;
	  node->left->parent = node;
	}
      else
	{
	  _Map_insert(map, node->left, key, value, height + 1);
	}
    }
  else if(1 == side)
    {
      if(NULL == node->right)
	{
	  if(NULL == node->left)
	    {
	      node->height = 1;
	    }

	  node->right = MapNode_new();
	  node->right->key = key;
	  node->right->value = value;
	  node->right->parent = node;
	}
      else
	{
	  _Map_insert(map, node->right, key, value, height + 1);
	}
    }
  
  int left_height = (node->left == NULL) ? 0 : node->left->height+1;
  int right_height = (node->right == NULL) ? 0 : node->right->height+1;

  node->height = max(left_height, right_height);
}

void Map_set(Map* map, void* key, void* value)
{
  if(map->root == NULL)
    {
      map->root = MapNode_new();
      map->root->key = key;
      map->root->value = value;
    }
  else
    {
      _Map_insert(map, map->root, key, value, 0);
    }
}

MapNode* _Map_findMin(MapNode* node)
{
  if(node == NULL)
    {
      return NULL;
    }

  if(node->left != NULL)
    {
      return _Map_findMin(node->left);
    }

  return node;
}

MapNode* _Map_findMax(MapNode* node)
{
  if(node == NULL)
    {
      return NULL;
    }

  if(node->right != NULL)
    {
      return _Map_findMin(node->right);
    }

  return node;
}

MapNode* _Map_find(Map* map, MapNode* node, void* key)
{
  if(node == NULL)
    {
      return NULL;
    }

  int side = map->compareKeys(key, node->key);

  if(0 == side)
    {
      return node;
    }
  else if(-1 == side)
    {
      return _Map_find(map, node->left, key);
    }
  else if(1 == side)
    {
      return _Map_find(map, node->right, key);
    }
}

void* Map_get(Map* map, void* key)
{
  MapNode* result = _Map_find(map, map->root, key);
  
  if(NULL == result)
    {
      return NULL;
    }
  else
    {
      return result->value;
    }
}

MapNode* Map_findByKey(Map* map, void* key)
{
    MapNode* node;

    return NULL;
}

MapNode* Map_findByValue(Map* map, void* value)
{
    MapNode* node;

    return NULL;
}


void _Map_remove(Map* map, MapNode* node, void* key)
{
  if(node == NULL)
    {
      return;
    }

  int side = map->compareKeys(key, node->key);

  if(-1 == side)
    {      
      _Map_remove(map, node->left, key);
    }
  else if(1 == side)
    {
      _Map_remove(map, node->right, key);
    }
  else if(0 == side)
    {
      if(node->left != NULL && node->right != NULL)
	{
	  MapNode* tmp = _Map_findMin(node->right);
	  MapNode* right = node->right;
	  
	  node->key = tmp->key;
	  node->value = tmp->value;

	  _Map_remove(map, right, tmp->key);
	}
      else
	{
	  if(node->left == NULL && node->right != NULL)
	    {
	      MapNode_copy(node, node->right);
	      MapNode_delete(node->right);
	    }
	  else if(node->right == NULL && node->left != NULL)
	    {
	      MapNode_copy(node, node->left);
	      MapNode_delete(node->left);
	    }
	  else {
	    if(node->parent != NULL && node == node->parent->left)
	      {
		node->parent->left = NULL;
		MapNode_delete(node);
	      }
	    else if(node->parent != NULL && node == node->parent->right)
	      {
		node->parent->right = NULL;
		MapNode_delete(node);
	      }
	  }
	}
    }

  int left_height = (node->left == NULL) ? 0 : node->left->height+1;
  int right_height = (node->right == NULL) ? 0 : node->right->height+1;

  node->height = max(left_height, right_height);
}

void Map_remove(Map* map, void* key)
{
  MapNode* node = _Map_find(map, map->root, key);

  if(NULL == node)
    {
      return;
    }
  else
    {
      _Map_remove(map, map->root, key);
    }
}

void _Map_print(MapNode* node, int level)
{
  if(NULL != node)
    {
      _Map_print(node->left, level+1);
      int i;
      for(i=0; i<level; i++)
	printf("   ");
      printf(" /\n");
      
      for(i=0; i<level; i++)
	printf("   ");
      printf("%d[%d]=%d\n", (int)node->key, node->height, (int)node->value);
      
      for(i=0; i<level; i++)
	printf("   ");
      printf(" \\\n");

      _Map_print(node->right, level+1);
    }
}

void Map_print(Map* map)
{
  _Map_print(map->root, 0);
}
