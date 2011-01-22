#include <smugstd.h>
#include <utils/orderedset.h>
#include <utils/binarysearchtree.h>

OrderedSet* OrderedSet_new(void)
{
    OrderedSet* set = malloc(sizeof(OrderedSet));

    OrderedSet_init(set);

    return set;
}

void OrderedSet_init(OrderedSet* set)
{
    BinarySearchTree_init((BinarySearchTree*) set);
}

void OrderedSet_delete(OrderedSet* set)
{
    free(set);
}

void OrderedSet_insert(OrderedSet* set, void* element)
{
    BinarySearchTree_insert((BinarySearchTree*) set, element);
}

void OrderedSet_setCompare(OrderedSet* set, int (*compare)(void*, void*, void*))
{
    BinarySearchTree_setCompare((BinarySearchTree*) set, NULL, compare);
}

void* OrderedSet_remove(OrderedSet* set, void* element)
{
    return BinarySearchTree_remove((BinarySearchTree*) set, element);
}

LinkedList* OrderedSet_removeIf(OrderedSet* set, BOOL (*predicate)(void*, void*), void* param)
{
    return BinarySearchTree_removeIf((BinarySearchTree*) set, predicate, param);
}

void* OrderedSet_find(OrderedSet* set, void* element)
{
    return BinarySearchTree_find((BinarySearchTree*) set, element);
}

LinkedList* OrderedSet_findIf(OrderedSet* set, BOOL (*predicate)(void*, void*), void* param)
{
    return BinarySearchTree_findIf((BinarySearchTree*) set, predicate, param);
}

void* OrderedSet_popMin(OrderedSet* set)
{
    void* element = BinarySearchTree_findMin((BinarySearchTree*) set);
    BinarySearchTree_remove((BinarySearchTree*) set, element);
    return element;
}

void OrderedSet_print(OrderedSet* set)
{
    BinarySearchTree_print((BinarySearchTree*) set);
}



OrderedSetIterator* OrderedSetIterator_new(void)
{
    OrderedSetIterator* iter = malloc(sizeof(OrderedSetIterator));

    OrderedSetIterator_init(iter);

    return iter;
}


void OrderedSetIterator_init(OrderedSetIterator* iter)
{
    BinarySearchTreeIterator_init((BinarySearchTreeIterator*) iter);
}


void OrderedSetIterator_delete(OrderedSetIterator* iter)
{
    if(NULL == iter)
    {
        return;
    }

    free(iter);
}


void OrderedSetIterator_reset(OrderedSet* set, OrderedSetIterator* iter)
{
    BinarySearchTreeIterator_reset((BinarySearchTree*) set, (BinarySearchTreeIterator*) iter);
}


void OrderedSetIterator_step(OrderedSetIterator* iter)
{
    BinarySearchTreeIterator_step((BinarySearchTreeIterator*) iter);
}


BOOL OrderedSetIterator_valid(OrderedSetIterator* iter)
{
    return BinarySearchTreeIterator_valid((BinarySearchTreeIterator*) iter);
}


void* OrderedSetIterator_get(OrderedSetIterator* iter)
{
    return BinarySearchTreeIterator_get((BinarySearchTreeIterator*) iter);
}


