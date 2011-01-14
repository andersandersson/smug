#include <stdio.h>                                       
#include "test-utils/assert.h"                           

#include "map.h"

int compare(int x, int y)
{
    if(x == y)
      {
	return 0;
      }

    if(x > y)
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
    Map* tree = Map_new();

    Map_get(tree, (void*)2);
 
    Map_setCompare(tree, compare);
    
    Map_get(tree, (void*)2);

    Map_set(tree, (void*)3, (void*)1);
    Map_set(tree, (void*)10,(void*)2);
    Map_set(tree, (void*)2, (void*)3);
    Map_set(tree, (void*)1, (void*)4);
    Map_set(tree, (void*)8, (void*)5);
    Map_set(tree, (void*)9, (void*)6);
    Map_set(tree, (void*)7, (void*)7);

    MapIterator* iter = MapIterator_new();

    int i = 0;
    for(MapIterator_reset(tree, iter); MapIterator_valid(iter) && i++ < 10; MapIterator_step(iter))
      {
	Pair* element = (Pair*) MapIterator_get(iter);
	
	printf("Now at %d = %d\n", element->left, element->right);
      }

    Pair p;
    p = Map_remove(tree, (void*)8);
    printf("Got from remove 8: %d = %d\n", p.left, p.right);

    p = Map_remove(tree, (void*)7);
    printf("Got from remove 7: %d = %d\n", p.left, p.right);
    p = Map_remove(tree, (void*)7);
    p = Map_remove(tree, (void*)7);

    printf("Getting 10: %d\n", (int) Map_get(tree, (void*)10));
    printf("Getting 10: %d\n", (int) Map_get(tree, (void*)2));
    printf("Getting 10: %d\n", (int) Map_get(tree, (void*)8));

    Map_delete(tree);
    return 0;
}
