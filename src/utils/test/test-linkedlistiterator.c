#include <stdio.h>
#include <stdlib.h>

#include <utils/linkedlist.h>
#include <utils/linkedlistiterator.h>

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

int maina()
{
    LinkedList* l;

    l = LinkedList_new();

	LinkedListIterator* iter = LinkedList_getIterator(l);
	if (LinkedListIterator_hasMore(iter))
	{
		return 1;
	}
	if (LinkedListIterator_getNext(iter) != NULL)
	{
		return 2;
	}
	if (LinkedListIterator_getSame(iter) != NULL)
	{
		return 3;
	}
	LinkedListIterator_delete(iter);

	int i;
	int* j;
    // Add 100 integer pointers to list.
    for (i = 0; i < 100; i++)
    {
        j = malloc(sizeof(int));
        *j = i;
        LinkedList_addLast(l, j);
    }
	iter = LinkedList_getIterator(l);
	int count = 0;
	while (LinkedListIterator_hasMore(iter))
	{
		j = LinkedListIterator_getNext(iter);
		if (count != *j)
		{
			return 4;
		}
		count++;
		if (j != LinkedListIterator_getSame(iter))
		{
			return 5;
		}
	}
	if (count != 99)
	{
		return 6;
	}
	LinkedListIterator_delete(iter);
    LinkedList_deleteContents(l, intDeleter);
    LinkedList_delete(l);

    return 0;
}
