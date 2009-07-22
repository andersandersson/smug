#include "linkedlist.h"

#include <stdlib.h>

void LinkedList_clear(LinkedList* list)
{
	Node* node = list->first;
	Node* next_node;

	while(NULL != node) 
	{
		next_node = node->next;
		Node_delete(node);
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

void LinkedList_addFirst(LinkedList* list, void* item)
{
	Node* node = Node_new();
  
	if (list->first)
	{
		node->next = list->first;
		list->first->prev = node;
	}
	else
	{
		list->last = node;
	}
	
	list->first = node;
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
	
	Node_delete(node);
}

void LinkedList_doList(LinkedList* list, void (*func)(void*))
{
	Node* node = list->first;
	while(NULL != node)
	{
		func(node->item);
		node = node->next;
	}
}

void LinkedList_doListIf(LinkedList* list, void (*func)(void*), BOOL(*pred)(void*))
{
	Node* node = list->first;
	while(NULL != node)
	{
		if (pred(node->item))
		{
			func(node->item);
		}
		node = node->next;
	}
}

LinkedList* LinkedList_getThose(LinkedList* list, BOOL(*pred)(void*))
{
	LinkedList* newList = LinkedList_new();
	Node* node = list->first;
	while (NULL != node)
	{
		if (pred(node->item))
		{
			LinkedList_addLast(newList, node->item);
		}
		node = node->next;
	}
	return newList;
}

LinkedList* LinkedList_map(LinkedList* list, void* (*func)(void*))
{
	LinkedList* newList = LinkedList_new();
	Node* node = list->first;
	while (NULL != node)
	{
		LinkedList_addLast(newList, func(node->item));
		node = node->next;
	}
	return newList;	
}

BOOL LinkedList_forAll(LinkedList* list, BOOL(*pred)(void*))
{
	Node* node = list->first;
	while(NULL != node)
	{
		if (!pred(node->item))
		{
			return FALSE;
		}
		node = node->next;
	}
	return TRUE;
}

BOOL LinkedList_exists(LinkedList* list, BOOL(*pred)(void*))
{
	Node* node = list->first;
	while(NULL != node)
	{
		if (pred(node->item))
		{
			return TRUE;
		}
		node = node->next;
	}
	return FALSE;
}

void LinkedList_deleteContents(LinkedList* list, void (*deleter)(void*))
{
	//LinkedList_traverse(list, deleter);
	LinkedList_clear(list);
}
