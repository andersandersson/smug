/** @file LinkedList.h
  * @brief Defines a regular linked list type (and a node type for it).
  */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/** A struct for the node type in a linked list.
  *
  * Create new nodes with Node_new, delete them with Node_destroy. Set the content of the node by setting Node->item directly.
  */
typedef struct Node {
	void* item; /**< Pointer to the actual data in the node. */
	struct Node* next; /**< Next node in list. Null if end of list. */
	struct Node* prev; /**< Previous node in list. Null if start of list */
} Node;

/** A struct for a linked list.
  *
  * Navigate the list by getting the Node->first pointer, and then using ->next on it until you get null.
  */
typedef struct LinkedList {
	Node* first; /**< First node in list. */
	Node* last; /**< Last node in list */
} LinkedList;

/** Creates and returns a new empty node
  *
  * \relatesalso Node
  */
Node* Node_new();

/** Destroys a node struct
  *
  * @relatesalso Node
  */
void Node_delete(Node* node);

/** Creates and returns a new empty list
  *
  * @relatesalso LinkedList
  */
LinkedList* LinkedList_new();

/** Destroys a list.
  * 
  * The data in the list is not deleted. Call LinkedList_deleteContents first to avoid memory leak.
  *
  * @relatesalso LinkedList
  */
void LinkedList_delete(LinkedList* list);

/** Adds the passed item to a new node at the end of the list.
  *
  * @relatesalso LinkedList
  */
void LinkedList_addLast(LinkedList* list, void* item);

/** Calls the passed function on every data item in the list.
  *
  * @relatesalso LinkedList
  */
void LinkedList_traverse(LinkedList* list, void (*func)(void*));

/** Removes the passed node from the list. Does not delete the contents of the node!
  *
  * \relatesalso LinkedList
  */
void LinkedList_remove(LinkedList* list, Node* node);

/** Deletes the contents of the list.
  * 
  * Calls the passed function on every data item in the list and then empties
  * the list. Assumes the passed function deletes the stored datatype correctly.
  *
  * @relatesalso LinkedList
  */
void LinkedList_deleteContents(LinkedList* list, void (*deleter)(void*));

/*
Create the deleter function like this:
ContentItem_destroy(void* contentItem) {
	free((ContentItem*)contentItem);
}
*/
#endif // LINKEDLIST_H
