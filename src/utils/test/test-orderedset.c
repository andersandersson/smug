#include <stdio.h>                                       
#include "test-utils/assert.h"                           

#include "orderedset.h"

int compare(void* tree, int x, int y)
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

int main(void)
{
    OrderedSet* tree = OrderedSet_new();
 
    OrderedSet_setCompare(tree, compare);

    OrderedSet_insert(tree, (void*)3);
    OrderedSet_insert(tree, (void*)10);
    OrderedSet_insert(tree, (void*)2);
    OrderedSet_insert(tree, (void*)1);
    OrderedSet_insert(tree, (void*)8);
    OrderedSet_insert(tree, (void*)9);
    OrderedSet_insert(tree, (void*)7);

    OrderedSetIterator* iter = OrderedSetIterator_new();

    printf("===========================\n");
    OrderedSet_print(tree);
    printf("===========================\n");

    int i = 0;
    for(OrderedSetIterator_reset(tree, iter); OrderedSetIterator_valid(iter) && i++ < 10; OrderedSetIterator_step(iter))
      {
	int element = (int) OrderedSetIterator_get(iter);
	
	printf("Now at %d\n", element);
      }

    printf("===========================\n");
    OrderedSet_popMin(tree);
    OrderedSet_print(tree);
    printf("===========================\n");
    OrderedSet_popMin(tree);
    OrderedSet_print(tree);
    printf("===========================\n");
    OrderedSet_popMin(tree);
    OrderedSet_print(tree);
    printf("===========================\n");
    OrderedSet_popMin(tree);
    OrderedSet_print(tree);

    OrderedSet_delete(tree);
    return 0;
}
