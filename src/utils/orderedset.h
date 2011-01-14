#ifndef SMUG_UTILS_ORDEREDSET_H
#define SMUG_UTILS_ORDEREDSET_H

#include <common/common.h>

struct BinarySearchTree;
struct BinarySearchTreeIterator;

typedef struct OrderedSet
{
    struct BinarySearchTree* tree;
} OrderedSet;


typedef struct OrderedSetIterator 
{
    struct BinarySearchTreeIterator* iterator;
} OrderedSetIterator;


OrderedSet* OrderedSet_new(void);
void OrderedSet_delete(OrderedSet* set);
void OrderedSet_setCompare(OrderedSet* set, int (*compare)(void*, void*));
void OrderedSet_insert(OrderedSet* set, void* element);
void OrderedSet_remove(OrderedSet* set, void* element);
void* OrderedSet_find(OrderedSet* set, void* element);
void OrderedSet_print(OrderedSet* set);


OrderedSetIterator* OrderedSetIterator_new(void);
void OrderedSetIterator_delete(OrderedSetIterator* iter);
void OrderedSetIterator_reset(OrderedSet* set, OrderedSetIterator* iter);
void OrderedSetIterator_step(OrderedSetIterator* iter);
BOOL OrderedSetIterator_valid(OrderedSetIterator* iter);
void* OrderedSetIterator_get(OrderedSetIterator* iter);


#endif // SMUG_UTILS_ORDEREDSET_H
