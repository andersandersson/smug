#ifndef PHYSICS_H
#define PHYSICS_H

#include "body.h"
#include "utils/hook.h"

typedef struct CollisionData
{
    Body* left;
    Body* right;
    Vector* result;
} CollisionData;

int Physics_init();
void Physics_terminate();

BOOL Physics_intervalOverlap(float left_l, float left_r, float right_l, float right_r, float* dest);

void Physics_addCollisionHook(BODY_TYPE left, BODY_TYPE right, Hook* hook);
void Physics_addBody(Body* body);
void Physics_removeBody(Body* body);
void Physics_update(TIME time);

BOOL Physics_collideRectangleRectangle(Body* left, Body* right, Vector* result);
void Physics_detectCollisions(LinkedList* left, LinkedList* right, LinkedList* collisionHooks);

#endif
