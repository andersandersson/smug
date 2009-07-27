#include <stdio.h>

#include "vector.h"

int main()
{
    Vector u, v, w;

    v = Vector_Create2d(1, 1);
    w = Vector_Create2d(2, 3);

    u = Vector_Add(v, w);

    if(u.x[0] != 3 || u.x[1] != 4) {
        printf("Vector addition failed.\n");
        return 1;
    }
    
    return 0;
}
