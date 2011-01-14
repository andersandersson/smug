#include <stdio.h>                                       
#include "test-utils/assert.h"                           

#include "map.h"

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
    Map* map = Map_new();
 
    map->compareKeys = compare;

    Map_set(map, (void*)3, (void*)15);
    Map_set(map, (void*)10, (void*)12);
    Map_set(map, (void*)2, (void*)13);
    Map_set(map, (void*)1, (void*)11);
    Map_set(map, (void*)8, (void*)16);
    Map_set(map, (void*)9, (void*)14);
    Map_set(map, (void*)7, (void*)20);


    printf("===========================\n");
    Map_print(map);

    Map_remove(map, (void*)8);

    printf("===========================\n");
    Map_print(map);

    Map_remove(map, (void*)7);

    printf("===========================\n");
    Map_print(map);
    printf("Getting 10: %d\n", (int) Map_get(map, (void*)10));
    Map_delete(map);
    return 0;
}
