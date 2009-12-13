/** @file physics.h
  * @brief Main file of the physics module. Collision detection etc.
  * @ingroup smug_physics
  */

/** @defgroup smug_physics Physics
  *
  * @{
  */

#ifndef PHYSICS_H
#define PHYSICS_H

#include "body.h"
#include "utils/hook.h"

typedef struct CollisionData
{
    Body* left;
    Body* right;
    Vector movement;
    float collisionTime;
} CollisionData;

int Physics_init();
BOOL Physics_isInitialized();
void Physics_terminate();

BOOL Physics_intervalOverlap(float left_l, float left_r, float right_l, float right_r, float* dest);

void Physics_addCollisionHook(BODY_TYPE left, BODY_TYPE right, Hook* hook);
void Physics_addBody(Body* body);
void Physics_removeBody(Body* body);
void Physics_update(TIME time);

BOOL Physics_collideRectangleRectangle(Body* left, Body* right, CollisionData** result);
void Physics_detectCollisions(LinkedList* left, LinkedList* right, LinkedList* collisions);

#endif

/**@}*/
