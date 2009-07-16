#include "LinkedList.h"

#include <stdlib.h>

void LinkedList_clear(LinkedList* list)
{
	Node* node = list->first;
	Node* next_node;

	while(NULL != node) 
	{
		next_node = node->next;
		Node_destroy(node);
		node = next_node;
	}

	list->first = NULL;
	list->last = NULL;
}

Node* Node_new()
{
	Node* node = malloc(sizeof(Node));

	node->item = NULL;
	node->next = NULL;
	node->prev = NULL;

	return node;
}

void Node_delete(Node* node)
{
//	free(node->item);
	free(node);
}

LinkedList* LinkedList_new()
{
	LinkedList* new_list;

	new_list = malloc(sizeof(LinkedList));

	new_list->first = NULL;
	new_list->last = NULL;

	return new_list;
}

void LinkedList_delete(LinkedList* list)
{
	// Node* node = list->first;
	// Node* next_node;

	// list->first = NULL;
	// list->last = NULL;

	// while(NULL != node) 
	// {
		// next_node = node->next;
		// Node_destroy(node);
		// node = next_node;
	// }
	LinkedList_clear(list);
	free(list);	
}

void LinkedList_addLast(LinkedList* list, void* item)
{
	Node* node = Node_new();
  
	if (list->last)
	{
		node->prev = list->last;
		list->last->next = node;
	}
	else
	{
		list->first = node;
	}
	
	list->last = node;
	node->item = item;
}

void LinkedList_remove(LinkedList* list, Node* node)
{
	// if (node == NULL) WARNING("LinkedList_remove: 'node' is NULL.")
	// if (list == NULL) WARNING("LinkedList_remove: 'list' is NULL.")
	// if (list->first == NULL || list->last == NULL) WARNING("LinkedList_remove: 'list' is empty.")
	
	// Node* next_node;
	// next_node = node->next;
	
	if (node == list->first)
	{
		list->first = node->next;
	}
	else
	{
		node->prev->next = node->next;
	}
	
	if (node == list->last)
	{
		list->last = node->prev;
	}
	else
	{
		node->next->prev = node->prev;
	}	
	
	Node_destroy(node);		
}

void LinkedList_traverse(LinkedList* list, void (*func)(void*))
{
	Node* node = list->first;
	while(NULL != node)
	{
		func(node->item);
		node = node->next;
	}
}

void LinkedList_deleteContents(LinkedList* list, void (*deleter)(void*))
{
	LinkedList_traverse(list, deleter);
	LinkedList_clear(list);
}
