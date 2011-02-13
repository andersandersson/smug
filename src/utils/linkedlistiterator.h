/**
 * @file linkedlistiterator.h
 * @brief Defines an iterator for the linked list.
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_LINKEDLISTITERATOR_H
#define SMUG_UTILS_LINKEDLISTITERATOR_H

#include <common/common.h>

struct LinkedList;
struct Node;

/**
 * An iterator for a LinkedList.
 * @sa ::LinkedList
 */
typedef struct LinkedListIterator
{
    struct Node* mCurrent;
    BOOL mHasReturnedFirst;
} LinkedListIterator;

/**
 * Creates and returns an iterator for the supplied list.
 *
 * The list should not be modified while the iterator is in use.
 * @relates LinkedListIterator
 */
LinkedListIterator* LinkedList_getIterator(struct LinkedList* self);

/**
 * Check whether there are more elements in the list.
 * @relates LinkedListIterator
 */
BOOL LinkedListIterator_hasMore(LinkedListIterator* self);

/**
 * Returns the next element in the list.
 * @relates LinkedListIterator
 */
void* LinkedListIterator_getNext(LinkedListIterator* self);

/**
 * Return the last element returned from getNext or getSame again.
 * @relates LinkedListIterator
 */
void* LinkedListIterator_getSame(LinkedListIterator* self);

/**
 * Delete the iterator.
 * @relates LinkedListIterator
 */
void LinkedListIterator_delete(LinkedListIterator* self);

#endif /* SMUG_UTILS_LINKEDLISTITERATOR_H */

/**@}*/
