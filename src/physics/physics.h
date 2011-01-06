/** @file physics.h
  * @brief Main file of the physics module. Collision detection etc.
  * @ingroup smug_physics
  */

/** @defgroup smug_physics Physics
  *
  * @{
  */

#ifndef SMUG_PHYSICS_PHYSICS_H
#define SMUG_PHYSICS_PHYSICS_H

#include <utils/hook.h>
#include <physics/body.h>

/** Data container for a collision
 *
 * This data is sent as a parameter to the collision handler
 * when two objects collide with eachother
 */
typedef struct CollisionData
{
    Body* left;
    Body* right;
    Vector movement;
    Vector normal;
    float collisionTime;
} CollisionData;

CollisionData* CollisionData_new(void);
void CollisionData_delete(CollisionData* data);

/** Initialize the physics
 */
int Physics_init(void);

/** Check if the Physics is initialized
 */
BOOL Physics_isInitialized(void);

/** Clean up the physics
 */
void Physics_terminate(void);

void Physics_addCollisionHook(BODY_TYPE left, BODY_TYPE right, Hook* hook);
void Physics_addBody(Body* body);
void Physics_removeBody(Body* body);
void Physics_update(TIME time);

#endif /* SMUG_PHYSICS_PHYSICS_H */

/**@}*/
