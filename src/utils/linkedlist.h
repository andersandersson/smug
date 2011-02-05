/**
 * @file linkedlist.h
 * @brief Defines a regular linked list type (and a node type for it).
 * @ingroup smug_utils
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_LINKEDLIST_H
#define SMUG_UTILS_LINKEDLIST_H

#include <common/common.h>

/**
 * A struct for the node type in a linked list.
 * Create new nodes with Node_new, delete them with Node_destroy. Set the content of the node by setting Node->item directly.
 * @sa ::LinkedList
 */
typedef struct Node {
    void* item;         /**< Pointer to the actual data in the node. */
    struct Node* next;     /**< Next node in list. Null if end of list. */
    struct Node* prev;     /**< Previous node in list. Null if start of list */
} Node;

/** A struct for a linked list.
 *
 * Navigate the list by getting the Node->first pointer, and then using ->next on it until you get null.
 *
 * @sa ::Node
 */
typedef struct LinkedList {
    Node* first;         /**< First node in list. */
    Node* last;         /**< Last node in list */
    int length;
} LinkedList;

/** Creates and returns a new empty node.
 *
 * @relatesalso Node
 * @return A pointer to the node just created.
 */
Node* Node_new(void);

/** Destroys a node struct
 *
 * @relatesalso Node
 * @param node A pointer to the node to be deleted.
 */
void Node_delete(Node* node);

/** Creates and returns a new empty list.
 *
 * @relatesalso LinkedList
 * @return A pointer to the list just created.
 */
LinkedList* LinkedList_new(void);

/** Destroys a list.
 *
 * The data in the list is not deleted. Call LinkedList_deleteContents first to avoid memory leak.
 *
 * @relatesalso LinkedList
 * @param self A pointer to the list to be deleted.
 */
void LinkedList_delete(LinkedList* self);

/**
 * Check if a list is empty or not.
 * @relatesalso LinkedList
 */
BOOL LinkedList_isEmpty(LinkedList* self);

/**
 * Get the length of a list.
 * @relatesalso LinkedList
 */
int LinkedList_length(LinkedList* self);

/** Adds the passed item to a new node at the end of the list.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param item A pointer to the item to be added.
 */
void LinkedList_addLast(LinkedList* self, void* item);

/** Adds the passed item to a new node at the beginning of the list.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param item A pointer to the item to be added.
 */
void LinkedList_addFirst(LinkedList* self, void* item);

/** Insert an item at a particular place in a list.
 *
 * Inserts the provided item right after the provided node in the list.
 *
 * @param self The list.
 * @param node A node in 'self' after which the new item will be inserted.
 * @param item A pointer to the item to be added.
 * @relatesalso LinkedList
 */
void LinkedList_insertAfter(LinkedList* self, Node* node, void* item);

/** Insert an item at a particular place in a list.
 *
 * Inserts the provided item right before the provided node in the list.
 *
 * @param self The list.
 * @param node A node in 'self' before which the new item will be inserted.
 * @param item A pointer to the item to be added.
 * @relatesalso LinkedList
 */
void LinkedList_insertBefore(LinkedList* self, Node* node, void* item);

/** Removes the passed node from the list. Does not delete the contents of the node!
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param node The node to remove.
 */
void LinkedList_remove(LinkedList* self, Node* node);

/** Removes the passed item from the list. Does not delete the actual item!
 *
 * Compares pointers to determine equality.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param item The item to remove.
 * @return TRUE if an item was removed.
 */
BOOL LinkedList_removeItem(LinkedList* self, void* item);

/**
 * Get the last item in a list.
 * @relatesalso LinkedList
 */
void* LinkedList_getLast(LinkedList* self);

/**
 * Get the first item in a list.
 * @relatesalso LinkedList
 */
void* LinkedList_getFirst(LinkedList* self);

/**
 * Get the last item in a list, then remove it from the list.
 * @relatesalso LinkedList
 */
void* LinkedList_popLast(LinkedList* self);

/**
 * Get the first item in a list, then remove it from the list.
 * @relatesalso LinkedList
 */
void* LinkedList_popFirst(LinkedList* self);

/** Deletes the contents of the list.
 *
 * Calls the passed function on every data item in the list and then empties
 * the list. Assumes the passed function deletes the stored datatype correctly.
 * The list still exists, empty, after the call.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param deleter The destructor function for the type the list holds.
 */
void LinkedList_deleteContents(LinkedList* self, void (*deleter)(void*));
/* Create the deleter function like this:
ContentItem_destroy(void* contentItem) {
    free((ContentItem*)contentItem);
}*/

/** Makes a deep copy of a linked list.
 *
 * @param itemCopier a function that copies the type of item the list contains.
 * @return A new list, just like the old one, containing copies of the elements in the old list.
 * @relatesalso LinkedList
 */
LinkedList* LinkedList_deepCopy(LinkedList* self, void* (*itemCopier)(void*));

/** Calls the passed function on every data item in the list.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param func The function to call.
 */
void LinkedList_doList(LinkedList* self, void (*func)(void*));

/** A conditional doList.
 *
 * Same as doList, but only executes the function for those elements that satisfy the predicate function.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param func The function to call.
 * @param pred The predicate function.
 */
void LinkedList_doListIf(LinkedList* self, void (*func)(void*), BOOL (*pred)(void*));

/** Returns a new list of those elements in list matching the predicate function.
 *
 * Returns a new list comprising those elements in list for which the predicate
 * function returns true. The actual elements are note copied, only the
 * pointers to them.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param pred The predicate function.
 */
LinkedList* LinkedList_getThose(LinkedList* self, BOOL (*pred)(void*));

/** Returns a mapping of the list according to a mapping function.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param func The mapping function.
 */
LinkedList* LinkedList_map(LinkedList* self, void* (*func)(void*));

/** Returns true if all the list elements satisfy the predicate.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param pred The predicate function.
 */
BOOL LinkedList_forAll(LinkedList* self, BOOL(*pred)(void*));

/** Returns true if at least one of the list elements satisfies the predicate.
 *
 * @relatesalso LinkedList
 * @param self The list.
 * @param pred The predicate function.
 */
BOOL LinkedList_exists(LinkedList* self, BOOL(*pred)(void*));

/** Concatenates two lists together.
 *
 * 'other' will be concatenated to the end of self. After this,
 * 'other' should be set to NULL and not used anymore.
 * @relatesalso LinkedList
 */
void LinkedList_concat(LinkedList* self, LinkedList* other);

/** Interleaves the items in a list with a particular item.
 *
 * The provided item is copied n - 1 times and the copies are inserted between each pair of items in
 * the list.
 *
 * @param itemCopier a function that copies the type of item the list contains.
 * @param item The item to interleave the list items with.
 * @relatesalso LinkedList
 */
void LinkedList_interleave(LinkedList* self, void* item, void* (*itemCopier)(void*));

#endif // SMUG_UTILS_LINKEDLIST_H

/**@}*/
