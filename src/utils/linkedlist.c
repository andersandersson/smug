#include "linkedlist.h"

#include <stdlib.h>

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
    free(node);
}

static void _clear(LinkedList* list)
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
    list->length = 0;
}

LinkedList* LinkedList_new()
{
    LinkedList* new_list;

    new_list = malloc(sizeof(LinkedList));

    new_list->first = NULL;
    new_list->last = NULL;
    new_list->length = 0;

    return new_list;
}

void LinkedList_delete(LinkedList* list)
{
    _clear(list);
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
    list->length++;
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
    list->length++;
}

void LinkedList_insertAfter(LinkedList* self, Node* node, void* item)
{
    Node* newNode = Node_new();
    newNode->item = item;
    newNode->next = node->next;
    newNode->prev = node;
    node->next = newNode;
}

void LinkedList_insertBefore(LinkedList* self, Node* node, void* item)
{
    Node* newNode = Node_new();
    newNode->item = item;
    newNode->next = node;
    newNode->prev = node->prev;
    node->prev = newNode;
}

BOOL LinkedList_isEmpty(LinkedList* list)
{
    return ((NULL == list) || ((NULL == list->first) && (NULL == list->last)));
    // OR: length == 0;
}

int LinkedList_length(LinkedList* list)
{
    return list->length;
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
    list->length--;
}

BOOL LinkedList_removeItem(LinkedList* list, void* item)
{
    Node* node = list->first;
    while(NULL != node)
    {
        if (node->item == item)
            {
                LinkedList_remove(list, node);
                return TRUE;
            }
        else
            {
                node = node->next;
            }
    }
    return FALSE;
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

void LinkedList_interleave(LinkedList* self, void* item, void* (*itemCopier)(void*))
{
    Node* iter;
    void* itemCopy;

    iter = self->first;
    iter = iter->next;

    while (NULL != iter)
    {
        LinkedList_insertBefore(self, iter, itemCopier(item));
        iter = iter->next;
    }
}

LinkedList* LinkedList_deepCopy(LinkedList* self, void* (*itemCopier)(void*))
{
    LinkedList* newList;
    Node* iter;

    newList = LinkedList_new();
    iter = self->first;

    while (NULL != iter)
    {
        LinkedList_addLast(newList, itemCopier(iter->item));
        iter = iter->next;
    }
    return newList;
}

void LinkedList_deleteContents(LinkedList* list, void (*deleter)(void*))
{
    //LinkedList_traverse(list, deleter);
    _clear(list);
}
