#include "rectangle.h"

#include "common/common.h"
#include <stdlib.h>

static void Rectangle_invariant(Rectangle* r)
{
    assert(NULL != r);
}

//Rectangle Rectangle_(double x, double y, double w, double h)
//{
//    Rectangle r;
//    r.x = x;
//    r.y = y;
//    r.w = w;
//    r.h = h;
//    return r;
//}
//

Rectangle* Rectangle_new(float x, float y, float w, float h)
{
    Rectangle* r = malloc(sizeof(Rectangle));
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
    Rectangle_invariant(r);
    return r;
}

void Rectangle_delete(Rectangle* r)
{
    Rectangle_invariant(r);
    free(r);
}
