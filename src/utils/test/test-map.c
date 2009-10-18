#include <stdio.h>                                       
#include "test-utils/assert.h"                           

#include "map.h"

int compare(int x, int y)
{
    fprintf(stderr, "%d == %d\n", x, y);

    return x == y ? 0 : 1;
}

int main()
{
    Map* map = Map_new();

    Map_set(map, (void*)23, (void*)11);

    map->compareKey = compare;

    
    fprintf(stderr, "Tal: %d\n",  (int)Map_get(map, (void*)23));
    fprintf(stderr, "Tal: %d\n",  (int)Map_get(map, (void*)21));

    Map_delete(map);
    return 0;
}
