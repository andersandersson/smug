#include <smugstd.h>
#include <utils/linkedlistiterator.h>
#include <utils/linkedlist.h>

LinkedListIterator* LinkedList_getIterator(LinkedList* self)
{
    LinkedListIterator* iter = (LinkedListIterator*)malloc(sizeof(LinkedListIterator));
    iter->mCurrent = self->first;
    iter->mHasReturnedFirst = FALSE;
    return iter;
}

void* LinkedListIterator_getNext(LinkedListIterator* self)
{
    if (!self->mHasReturnedFirst)
    {
        if (self->mCurrent != NULL)
        {
            self->mHasReturnedFirst = TRUE;
            return self->mCurrent->item;
        }
        else
        {
            return NULL;
        }
    }
    self->mCurrent = self->mCurrent->next;
    if (self->mCurrent != NULL)
    {
        return self->mCurrent->item;
    }
    return NULL;
}

void* LinkedListIterator_getSame(LinkedListIterator* self)
{
    smug_assert(self != NULL);
    if (self->mCurrent != NULL)
    {
        return self->mCurrent->item;
    }
    return NULL;
}

BOOL LinkedListIterator_hasMore(LinkedListIterator* self)
{
    return self->mCurrent != NULL && self->mCurrent->next != NULL;
}

void LinkedListIterator_delete(LinkedListIterator* self)
{
    free(self);
}