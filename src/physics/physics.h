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

/** Data container for a collision
 *
 * This data is sent as a parameter to the collision handler
 * when two objects collide with eachother.
 */
typedef struct CollisionData
{
    /** The left hand of the collision, and the object to modify position for. */
    Body* self;

    /** The object we collided with. Should probably not be altered. */
    Body* other;

    /** The attempted movement of "self". */
    Vector selfMovement;

    /** The normal for the surface of "other" that "self" collided with. */
    Vector contactNormal;

    /** A floating point number form 0.0 to 1.0 with the time for when along selfMovement the collision occured. */
    float collisionTime;
} CollisionData;


/** Allocate memory for a new CollisionData object
 * @relatesalso CollisionData
 */
CollisionData* CollisionData_new(void);


/** Free the memory for a CollisionData object
 * @relatesalso CollisionData
 */
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


/** Add a collision hook to be run on collision
 *
 * This function takes two BODY_TYPE and a hook. When two
 * bodies of the speficied types collide with eachother, the
 * hook is called with the corresponding CollisionData
 */
void Physics_addCollisionHook(BODY_TYPE self, BODY_TYPE other, Hook* hook);


/** Add a Body to the physics engine
 */
void Physics_addBody(Body* body);


/** Remove a Body from the physics engine
 */
void Physics_removeBody(Body* body);


/** Performe a tick in the engine
 *
 * Update the Physics world with a new timestamp. This function
 * will check for collision between all objects and call the
 * related collision hook. Afterwards it will update all objects
 * with their new positions.
 *
 * @param time Time since application start
 */
void Physics_update(TIME time);

#endif

/**@}*/
