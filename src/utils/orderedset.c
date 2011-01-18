#include <smugstd.h>
#include <utils/orderedset.h>
#include <utils/binarysearchtree.h>

OrderedSet* OrderedSet_new(void)
{
  OrderedSet* set = malloc(sizeof(OrderedSet));

  set->tree = BinarySearchTree_new();

  return set;
}

void OrderedSet_delete(OrderedSet* set)
{
  BinarySearchTree_delete(set->tree);

  free(set);
}

void OrderedSet_insert(OrderedSet* set, void* element)
{
  BinarySearchTree_insert(set->tree, element);
}

void OrderedSet_setCompare(OrderedSet* set, int (*compare)(void*, void*, void*))
{
  BinarySearchTree_setCompare(set->tree, NULL, compare);
}

void* OrderedSet_remove(OrderedSet* set, void* element)
{
  return BinarySearchTree_remove(set->tree, element);
}

void* OrderedSet_find(OrderedSet* set, void* element)
{
  return BinarySearchTree_find(set->tree, element);
}



OrderedSetIterator* OrderedSetIterator_new(void)
{
  OrderedSetIterator* iter = malloc(sizeof(OrderedSetIterator));

  iter->iterator = BinarySearchTreeIterator_new();

  return iter;
}


void OrderedSetIterator_delete(OrderedSetIterator* iter)
{
  if(NULL == iter)
    {
      return;
    }

  BinarySearchTreeIterator_delete(iter->iterator);
  
  free(iter);
}


void OrderedSetIterator_reset(OrderedSet* set, OrderedSetIterator* iter)
{
  BinarySearchTreeIterator_reset(set->tree, iter->iterator);
}


void OrderedSetIterator_step(OrderedSetIterator* iter)
{
  BinarySearchTreeIterator_step(iter->iterator);
}


BOOL OrderedSetIterator_valid(OrderedSetIterator* iter)
{
  return BinarySearchTreeIterator_valid(iter->iterator);
}


void* OrderedSetIterator_get(OrderedSetIterator* iter)
{
  return BinarySearchTreeIterator_get(iter->iterator);
}


