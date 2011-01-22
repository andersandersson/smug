#include <stdio.h>
#include "test-utils/assert.h"

#include "binarysearchtree.h"

int compare(void* data, int x, int y)
{
    if(x == y)
      {
	return 0;
      }

    if(x < y)
      {
	return -1;
      }
    else
      {
	return 1;
      }
}

BOOL predicate(void* left, void* right)
{
    if((int)left < (int)right)
        return TRUE;

    return FALSE;
}

int main(void)
{
    BinarySearchTree* tree = BinarySearchTree_new();

    BinarySearchTree_setCompare(tree, tree, compare);

    BinarySearchTree_insert(tree, (void*)3);
    BinarySearchTree_insert(tree, (void*)10);
    BinarySearchTree_insert(tree, (void*)2);
    BinarySearchTree_insert(tree, (void*)1);
    BinarySearchTree_insert(tree, (void*)8);
    BinarySearchTree_insert(tree, (void*)9);
    BinarySearchTree_insert(tree, (void*)7);

    LinkedList* list = BinarySearchTree_findIf(tree, predicate, (void*)4);
    Node* node;
    for(node = list->first; node != NULL; node = node->next)
    {
        printf("ITEM: %d\n", (int)node->item);
    }

    LinkedList_delete(list);

    BinarySearchTreeIterator* iter = BinarySearchTreeIterator_new();

    printf("===========================\n");
    BinarySearchTree_print(tree);
    printf("===========================\n");

    list = BinarySearchTree_removeIf(tree, predicate, (void*)4);
    LinkedList_delete(list);

    BinarySearchTree_print(tree);
    printf("===========================\n");

    int i = 0;
    for(BinarySearchTreeIterator_reset(tree, iter); BinarySearchTreeIterator_valid(iter) && i++ < 10; BinarySearchTreeIterator_step(iter))
      {
	int element = (int) BinarySearchTreeIterator_get(iter);

	printf("Now at %d\n", element);
      }

    BinarySearchTree_delete(tree);
    return 0;
}
