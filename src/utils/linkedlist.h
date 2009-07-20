/** @file LinkedList.h
  * @brief Defines a regular linked list type (and a node type for it).
  */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../engine/common.h"

/** A struct for the node type in a linked list.
  *
  * Create new nodes with Node_new, delete them with Node_destroy. Set the content of the node by setting Node->item directly.
  *
  * @sa ::LinkedList
  */
typedef struct Node {
	void* item; /**< Pointer to the actual data in the node. */
	struct Node* next; /**< Next node in list. Null if end of list. */
	struct Node* prev; /**< Previous node in list. Null if start of list */
} Node;

/** A struct for a linked list.
  *
  * Navigate the list by getting the Node->first pointer, and then using ->next on it until you get null.
  *
  * @sa ::Node
  */
typedef struct LinkedList {
	Node* first; /**< First node in list. */
	Node* last; /**< Last node in list */
} LinkedList;

/** Creates and returns a new empty node.
  *
  * @relatesalso Node
  * @return A pointer to the node just created.
  */
Node* Node_new();

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
LinkedList* LinkedList_new();

/** Destroys a list.
  * 
  * The data in the list is not deleted. Call LinkedList_deleteContents first to avoid memory leak.
  *
  * @relatesalso LinkedList
  * @param list A pointer to the list to be deleted.
  */
void LinkedList_delete(LinkedList* list);

/** Adds the passed item to a new node at the end of the list.
  *
  * @relatesalso LinkedList
  * @param list The list.
  * @param item A pointer to the item to be added.
  */
void LinkedList_addLast(LinkedList* list, void* item);

/** Calls the passed function on every data item in the list.
  *
  * @relatesalso LinkedList
  * @param list The list.
  * @param func The function to call.
  */
void LinkedList_doList(LinkedList* list, void (*func)(void*));

/** A conditional doList.
  *
  * Same as doList, but only executes the function for those elements that satisfy the predicate function.
  * 
  * @relatesalso LinkedList
  * @param list The list.
  * @param func The function to call.
  * @param pred The predicate function.
  */
void LinkedList_doListIf(LinkedList* list, void (*func)(void*), BOOL (*pred)(void*));

/** Returns a new list of those elements in list matching the predicate function.
  * 
  * Returns a new list comprising those elements in list for which the predicate
  * function returns true. The actual elements are note copied, only the
  * pointers to them.
  * 
  * @relatesalso LinkedList
  * @param list The list.
  * @param pred The predicate function.
  */
LinkedList* LinkedList_getThose(LinkedList* list, BOOL (*pred)(void*));

/** Returns a mapping of the list according to a mapping function.
  *
  * @relatesalso LinkedList
  * @param list The list.
  * @param func The mapping function.
  */
LinkedList* LinkedList_map(LinkedList* list, void* (*func)(void*));

/** Returns true if all the list elements satisfy the predicate.
  *
  * @relatesalso LinkedList
  * @param list The list.
  * @param pred The predicate function.
  */
BOOL LinkedList_forAll(LinkedList* list, BOOL(*pred)(void*));

/** Returns true if at least one of the list elements satisfies the predicate.
  *
  * @relatesalso LinkedList
  * @param list The list.
  * @param pred The predicate function.
  */
BOOL LinkedList_exists(LinkedList* list, BOOL(*pred)(void*));

/** Removes the passed node from the list. Does not delete the contents of the node!
  *
  * @relatesalso LinkedList
  * @param list The list.
  * @param node The node to remove.
  */
void LinkedList_remove(LinkedList* list, Node* node);

/** Deletes the contents of the list.
  * 
  * Calls the passed function on every data item in the list and then empties
  * the list. Assumes the passed function deletes the stored datatype correctly.
  *
  * @relatesalso LinkedList
  * @param list The list.
  * @param deleter The destructor function for the type the list holds.
  */
void LinkedList_deleteContents(LinkedList* list, void (*deleter)(void*));

/*
Create the deleter function like this:
ContentItem_destroy(void* contentItem) {
	free((ContentItem*)contentItem);
}
*/
#endif // LINKEDLIST_H
