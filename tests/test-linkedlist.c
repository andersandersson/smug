#include <stdio.h>

#include "utils/linkedlist.h"

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
	printf("\n");

	LinkedList* l;
	LinkedList* l2;

	l = LinkedList_new();
	
//	printf("LinkedList created.\n");
	
	int i;
	int* j;
	
	// Add 100 integer pointers to list.
	for (i = 0; i < 100; i++)
	{
		j = malloc(sizeof(int));
		*j = i;
		LinkedList_addLast(l, j);
	}
//	printf("LinkedList filled.\n");

	l2 = LinkedList_getThose(l, isEven);
	// printf("These should be the even numbers from 0 to 99:\n");
	// printList(l2);

	if (LinkedList_exists(l2, isOdd))
	{
		return 1;
	}
	if (! LinkedList_forAll(l2, isEven))
	{
		return 2;
	}
	
	LinkedList_delete(l2);
	LinkedList_deleteContents(l, intDeleter);
	if (! LinkedList_isEmpty(l))
	{
		return 3;
	}
	LinkedList_delete(l);
	
	printf("\n");
	
	return 0;
}
