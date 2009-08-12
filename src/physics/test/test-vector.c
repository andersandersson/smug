#include <stdio.h>

#include "test-utils/assert.h"
#include "vector.h"

int main()
{
    Vector* u = Vector_new();
    Vector* v = Vector_new2f(1.0, 2.0);
    Vector* w = Vector_new2f(1.0, 1.0);

    Vector_add(v, w, u);

    assertTrue(u->x[0] == 2.0);
    assertTrue(u->x[1] == 3.0);

    Vector_add(v, w, u);

    assertTrue(u->x[0] == 0.0);
    assertTrue(u->x[1] == -1.0);
    
    Vector_delete(w);
    Vector_delete(v);
    Vector_delete(u);
    return 0;
}
