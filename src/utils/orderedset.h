#ifndef SMUG_UTILS_ORDEREDSET_H
#define SMUG_UTILS_ORDEREDSET_H

#include <common/common.h>
#include <utils/binarysearchtree.h>

/** Orderd set structure
  *
  * A set data structure. Keeping a bunch of elements
  * ordered and uniquely.
  */
typedef struct OrderedSet
{
  struct BinarySearchTree base; //< Inherit BinarySearchTree
} OrderedSet;


/** Iterator for OrderedSet
  *
  * Struct for iterating over an OrderedSet
  */
typedef struct OrderedSetIterator
{
  struct BinarySearchTreeIterator base; //< Inherit BinarySearchTreeIterator
} OrderedSetIterator;


/** Create and return a new OrderedSet
  *
  * @relatesalso OrderedSet
  * @return The created OrderedSet
  */
OrderedSet* OrderedSet_new(void);


/** Initialize a set
  *
  * @relatesalso OrderedSet
  * @param set The set to initialize
  */
void OrderedSet_init(OrderedSet* set);


/** Delete an OrderedSet
 *
 * @relatesalso OrderedSet
 * @param set The set to delete
 */
void OrderedSet_delete(OrderedSet* set);


/** Set comparison function for the set
  *
  * @relatesalso OrderedSet
  * @sa ::BinarySearchTree_setCompare
  * @param set The set to set compare fore
  * @param compare The comparison function
  */
void OrderedSet_setCompare(OrderedSet* set, int (*compare)(void*, void*, void*));


/** Insert a new item into a set
  *
  * @relatesalso OrderedSet
  * @param set The set to insert into
  * @param element The element to isnert
  */
void OrderedSet_insert(OrderedSet* set, void* element);


/** Remove an item from a set
  *
  * @relatesalso OrderedSet
  * @param set The set to remove from
  * @param element The element to remove
  * @return A pointer to the removed element
  */
void* OrderedSet_remove(OrderedSet* set, void* element);


/** Conditionally remove items from a set
  *
  * @relatesalso OrderedSet
  * @param set The set to remove from
  * @param predicate The predicate function for matching elements
  * @param parma The left side of the comparison
  * @return A list pointer with removed elements
  */
LinkedList* OrderedSet_removeIf(OrderedSet* set, BOOL (*predicate)(void*, void*), void* param);


/** Search for an element in the set
  *
  * @relatesalso OrderedSet
  * @param set The set to search in
  * @param element The element to search for
  * @return The found element or NULL
  */
void* OrderedSet_find(OrderedSet* set, void* element);


/** Conditionally find items from a set
  *
  * @relatesalso OrderedSet
  * @param set The set to search in
  * @param predicate The predicate function for matching elements
  * @param parma The left side of the comparison
  * @return A list pointer with found elements
  */
LinkedList* OrderedSet_findIf(OrderedSet* set, BOOL (*predicate)(void*, void*), void* param);


/** Fetch and remove the least item
  *
  * Find the least item in the set, remove and
  * return it.
  *
  * @relatesalso OrderedSet
  * @param set The OrderedSet to pop from
  * @return The item found or NULL if the set is empty
  */
void* OrderedSet_popMin(OrderedSet* set);


/** Print the set to stdout
  *
  * @relatesalso OrderedSet
  * @param set The OrderedSet to print
  */
void OrderedSet_print(OrderedSet* set);


/** Create a new OrderedSetIterator
  *
  * @relatesalso OrderedSetIterator
  * @return The created OrderedSetIterator
  */
OrderedSetIterator* OrderedSetIterator_new(void);


/** Initializes an iterator
  *
  * @relatesalso OrderedSetIterator
  * @param iter The iterator to init
  */
void OrderedSetIterator_init(OrderedSetIterator* iter);


/** Delete an OrderedSetIterator
  *
  * @relatesalso OrderedSetIterator
  * @param iter The OrderedSetIterator to be deleted
  */
void OrderedSetIterator_delete(OrderedSetIterator* iter);


/** Reset an OrderedSetIterator as forward iterator
  *
  * @relatesalso OrderedSetIterator
  * @param set The set to iterate through
  * @param iter The iterator to reset
  */
void OrderedSetIterator_reset(OrderedSet* set, OrderedSetIterator* iter);


/** Step an OrderedSetIterator forward
  *
  * @relatesalso OrderedSetIterator
  * @param iter The iterator to step
  */
void OrderedSetIterator_step(OrderedSetIterator* iter);


/** Check validity of a OrderedSetIterator
  *
  * @relatesalso OrderedSetIterator
  * @param iter The OrderedSetIterator to check
  * @return TRUE or FALSE depending on validity
  */
BOOL OrderedSetIterator_valid(OrderedSetIterator* iter);


/** Get the current value of the iterator
  *
  * @relatesalso OrderedSetIterator
  * @param iter The OrderedSetIterator to get for
  * @return The value or NULL if the iterator is invalid
  */
void* OrderedSetIterator_get(OrderedSetIterator* iter);

#endif // SMUG_UTILS_ORDEREDSET_H
