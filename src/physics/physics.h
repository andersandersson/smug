#ifndef PHYSICS_H
#define PHYSICS_H

#include "common/log.h"

#include "vector.h"
#include "shape.h"

typedef struct Body
{
    Vector* position;
    Vector* offset;
    Shape* shape;
} Body;

int Physics_init();
void Physics_terminate();

void Physics_addBody(Body* body);
void Physics_removeBody(Body* body);

Body* Body_new();
void Body_delete(Body* body);
void Body_setShape(Shape* shape);
#endif
