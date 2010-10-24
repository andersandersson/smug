#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

void printList(LinkedList* l)
{
    Node* n = l->first;
    while (NULL != n)
    {
        printf("%i", *((int*)(n->item)));
        printf(" ");
        n = n->next;
    }
    printf("\n");
}

BOOL isEven(void* i)
{
    return (*(int*)i % 2) == 0;
}

BOOL isOdd(void* i)
{
    return (*(int*)i % 2) == 1;
}

void intDeleter(void* ip)
{
    free((int*)ip);
}

int main()
{
    LinkedList* l;
    LinkedList* l2;

    l = LinkedList_new();

    int i;
    int* j;

    // Add 100 integer pointers to list.
    for (i = 0; i < 100; i++)
    {
        j = malloc(sizeof(int));
        *j = i;
        LinkedList_addLast(l, j);
    }
    if (100 != LinkedList_length(l))
    {
        return 4;
    }

    l2 = LinkedList_getThose(l, isEven);

    if (LinkedList_exists(l2, isOdd))
    {
        return 1;
    }
    if (! LinkedList_forAll(l2, isEven))
    {
        return 2;
    }
    if (50 != LinkedList_length(l2))
    {
        return 5;
    }

    LinkedList_delete(l2);
    LinkedList_deleteContents(l, intDeleter);
    if (! LinkedList_isEmpty(l))
    {
        return 3;
    }

    for (i = 0; i < 100; i++)
    {
        j = malloc(sizeof(int));
        *j = i;
        LinkedList_addFirst(l, j);
    }
    if (100 != LinkedList_length(l))
    {
        return 6;
    }

    // TODO: Iterate over list and test remove
    // Test doList, doListIf and map.

    LinkedList_delete(l);

    return 0;
}
