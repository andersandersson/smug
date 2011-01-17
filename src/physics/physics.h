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

#include <common/common.h>
#include <utils/hook.h>
#include <utils/linkedlist.h>
#include <utils/vector_type.h>
#include <utils/orderedset.h>

#include <physics/body.h>

typedef unsigned int COLLISION_TYPE;

#define COLLISION_NONE    0x00
#define COLLISION_ENTER   0x01
#define COLLISION_PERSIST 0x02
#define COLLISION_EXIT    0x04

BOOL _collideRectangleRectangle(Body* self, Point* self_start, Vector* self_velocity, Vector* self_acceleration, 
				Body* other, Point* other_start, Vector* other_velocity, Vector* other_acceleration,
				TIME delta_time, LinkedList* collisions);

BOOL _detectCollisions(LinkedList* self, LinkedList* other, TIME delta_time, OrderedSet* collisions);

/** Data container for a collision
 *
 * This data is sent as a parameter to the collision handler
 * when two objects collide with eachother.
 */
typedef struct CollisionData
{
    /** The kind of collision, entering, exiting, persistent. */
    COLLISION_TYPE type;

    /** The left hand of the collision, and the object to modify position for. */
    Body* self;

    /** The object we collided with. Should probably not be altered. */
    Body* other;

    /** The normal for the surface of "other" that "self" collided with. */
    Vector normal;

    /** The self objects velocity */  
    Vector selfVelocity;

    /** The self starting point. May differ from self->position. */
    Point selfStart;
    Vector selfMovement;

    /** The other objects velocity */  
    Vector otherVelocity;

    Vector wayout;

    /** The self starting point. May differ from self->position. */
    Point otherStart;
    Vector otherMovement;

    /** A floating point number form 0.0 to 1.0 with the time for when along selfMovement the collision stopped. */
    TIME time;

    TIME absoluteTime;

    TIME absoluteTimeStart;

    TIME deltaTime;
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
void Physics_setCollisionHandler(BODY_TYPE self, BODY_TYPE other, COLLISION_TYPE type, Hook* hook);


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
void Physics_update(TIME time, BOOL do_update);


/** Add default handler Hook for solid, gliding objects
 */
void Physics_setDefaultCollisionHandler(BODY_TYPE left, BODY_TYPE right);

#endif /* SMUG_PHYSICS_PHYSICS_H */

/**@}*/
