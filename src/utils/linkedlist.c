#include <smugstd.h>

#include <common/common.h>

#include <utils/linkedlist.h>

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

static BOOL _invariant(LinkedList* self)
{
    BOOL ret = (self != NULL &&
            (   (self->first == NULL &&
                self->last == NULL)
                ||
                (self->first != NULL &&
                self->last != NULL &&
                self->first->prev == NULL &&
                self->last->next == NULL)
            )
        );
    if (!ret)
    {
        LinkedList* ll;
        ll->first = Node_new();
    }
    return ret;
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
    free(node);
}

LinkedList* LinkedList_new()
{
    LinkedList* new_list;

    new_list = malloc(sizeof(LinkedList));

    new_list->first = NULL;
    new_list->last = NULL;
    new_list->length = 0;

    smug_assert(_invariant(new_list));

    return new_list;
}

void LinkedList_delete(LinkedList* self)
{
    smug_assert(_invariant(self));

    _clear(self);
    free(self);
}

void LinkedList_addLast(LinkedList* self, void* item)
{
    smug_assert(_invariant(self));

    Node* node = Node_new();

    if (self->last)
    {
        node->prev = self->last;
        self->last->next = node;
    }
    else
    {
        self->first = node;
    }

    self->last = node;
    node->item = item;
    self->length++;
}

void LinkedList_addFirst(LinkedList* self, void* item)
{
    smug_assert(_invariant(self));

    Node* node = Node_new();

    if (self->first)
    {
        node->next = self->first;
        self->first->prev = node;
    }
    else
    {
        self->last = node;
    }

    self->first = node;
    node->item = item;
    self->length++;
}

void LinkedList_insertAfter(LinkedList* self, Node* node, void* item)
{
    smug_assert(_invariant(self));

    Node* newNode = Node_new();
    newNode->item = item;
    newNode->next = node->next;
    newNode->prev = node;
    node->next = newNode;
}

void LinkedList_insertBefore(LinkedList* self, Node* node, void* item)
{
    smug_assert(_invariant(self));

    Node* newNode = Node_new();
    newNode->item = item;
    newNode->next = node;
    newNode->prev = node->prev;
    node->prev = newNode;
}

void* LinkedList_getLast(LinkedList* self)
{
    return self->last->item;
}

void* LinkedList_popLast(LinkedList* self)
{
    void* data = self->last->item;
    LinkedList_remove(self, self->last);
    return data;
}

void* LinkedList_getFirst(LinkedList* self)
{
    return self->first->item;
}

void* LinkedList_popFirst(LinkedList* self)
{
    void* data = self->first->item;
    LinkedList_remove(self, self->first);
    return data;
}

BOOL LinkedList_isEmpty(LinkedList* self)
{
    smug_assert(_invariant(self));

    return (NULL == self->first && NULL == self->last);
}

int LinkedList_length(LinkedList* self)
{
    smug_assert(_invariant(self));

    return self->length;
}

void LinkedList_remove(LinkedList* self, Node* node)
{
    smug_assert(_invariant(self));

    if (node == self->first)
    {
        self->first = node->next;
    }
    else
    {
        node->prev->next = node->next;
    }

    if (node == self->last)
    {
        self->last = node->prev;
    }
    else
    {
        node->next->prev = node->prev;
    }

    Node_delete(node);
    self->length--;
}

BOOL LinkedList_removeItem(LinkedList* self, void* item)
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        if (node->item == item)
            {
                LinkedList_remove(self, node);
                return TRUE;
            }
        else
            {
                node = node->next;
            }
    }
    return FALSE;
}

void LinkedList_doList(LinkedList* self, void (*func)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        func(node->item);
        node = node->next;
    }
}

void LinkedList_doListIf(LinkedList* self, void (*func)(void*), BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        if (pred(node->item))
        {
            func(node->item);
        }
        node = node->next;
    }
}

LinkedList* LinkedList_getThose(LinkedList* self, BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    LinkedList* newList = LinkedList_new();
    Node* node = self->first;
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

LinkedList* LinkedList_map(LinkedList* self, void* (*func)(void*))
{
    smug_assert(_invariant(self));

    LinkedList* newList = LinkedList_new();
    Node* node = self->first;
    while (NULL != node)
    {
        LinkedList_addLast(newList, func(node->item));
        node = node->next;
    }
    return newList;
}

BOOL LinkedList_forAll(LinkedList* self, BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
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

BOOL LinkedList_exists(LinkedList* self, BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
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

void LinkedList_concat(LinkedList* self, LinkedList* other)
{
    smug_assert(_invariant(self));
    smug_assert(_invariant(other));
    self->last->next = other->first;
    other->first->prev = self->last;
}

void LinkedList_interleave(LinkedList* self, void* item, void* (*itemCopier)(void*))
{
    Node* iter;
    //void* itemCopy; is not used

    smug_assert(_invariant(self));

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

    smug_assert(_invariant(self));

    newList = LinkedList_new();
    iter = self->first;

    while (NULL != iter)
    {
        LinkedList_addLast(newList, itemCopier(iter->item));
        iter = iter->next;
    }
    return newList;
}

void LinkedList_deleteContents(LinkedList* self, void (*deleter)(void*))
{
    smug_assert(_invariant(self));
    //LinkedList_traverse(self, deleter);
    _clear(self);
}
