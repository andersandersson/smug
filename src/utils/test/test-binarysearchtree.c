#include <stdio.h>                                       
#include "test-utils/assert.h"                           

#include "binarysearchtree.h"

int compare(int x, int y)
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

int main()
{
    BinarySearchTree* tree = BinarySearchTree_new();
 
    BinarySearchTree_setCompare(tree, compare);

    BinarySearchTree_insert(tree, (void*)3);
    BinarySearchTree_insert(tree, (void*)10);
    BinarySearchTree_insert(tree, (void*)2);
    BinarySearchTree_insert(tree, (void*)1);
    BinarySearchTree_insert(tree, (void*)8);
    BinarySearchTree_insert(tree, (void*)9);
    BinarySearchTree_insert(tree, (void*)7);

    BinarySearchTreeIterator* iter = BinarySearchTreeIterator_new();

    printf("===========================\n");
    BinarySearchTree_print(tree);
    printf("===========================\n");

    int i = 0;
    for(BinarySearchTreeIterator_reset(tree, iter); BinarySearchTreeIterator_valid(iter) && i++ < 10; BinarySearchTreeIterator_step(iter))
      {
	int element = (int) BinarySearchTreeIterator_get(iter);
	
	printf("Now at %d\n", element);
      }

    printf("===========================\n");
    BinarySearchTree_print(tree);

    BinarySearchTree_remove(tree, (void*)8);

    printf("===========================\n");
    BinarySearchTree_print(tree);

    BinarySearchTree_remove(tree, (void*)7);

    printf("===========================\n");
    BinarySearchTree_print(tree);
    printf("Getting 10: %d\n", (int) BinarySearchTree_find(tree, (void*)10));
    BinarySearchTree_delete(tree);
    return 0;
}
