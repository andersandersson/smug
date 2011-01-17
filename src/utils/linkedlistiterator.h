#ifndef SMUG_UTILS_LINKEDLISTITERATOR_H
#define SMUG_UTILS_LINKEDLISTITERATOR_H

#include <common/common.h>

struct LinkedList;
struct Node;

typedef struct LinkedListIterator
{
    struct Node* mCurrent;
    BOOL mHasReturnedFirst;
} LinkedListIterator;

LinkedListIterator* LinkedList_getIterator(struct LinkedList* self);
BOOL LinkedListIterator_hasMore(LinkedListIterator* self);
void* LinkedListIterator_getNext(LinkedListIterator* self);
void* LinkedListIterator_getSame(LinkedListIterator* self);
void LinkedListIterator_delete(LinkedListIterator* self);

#endif /* SMUG_UTILS_LINKEDLISTITERATOR_H */
