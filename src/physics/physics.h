/**
 * @file physics.h
 * @brief Main file of the physics module. Collision detection etc.
 * @if doc_files
 * @ingroup smug_physics
 * @endif
 */

/**
 * @defgroup smug_physics Physics
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
#define COLLISION_EXIT    0x03
#define COLLISION_TYPE_LENGTH  0x04

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

    /** The bodies involved in the collision. */
    Body* body[2];

    /** The normal for the surface of "other" that "self" collided with. */
    Vector normal[2];

    /** The self objects velocity */
    Vector velocity[2];

    /** The accelerations of the bodies. */
    Vector acceleration[2];

    /** The self starting point. May differ from self->position. */
    Point start[2];

    /** The movement made by the bodies. */
    Vector movement[2];

    /** How to move the object out of collision */
    Vector wayout[2];

    /** A floating point number form 0.0 to 1.0 with the time for when along selfMovement the collision stopped. */
    TIME time;
    TIME absoluteTime;
    TIME absoluteTimeStart;
    TIME deltaTime;
} CollisionData;


/** Allocate memory for a new CollisionData object
 * @relates CollisionData
 */
CollisionData* CollisionData_new(void);


/** Free the memory for a CollisionData object
 * @relates CollisionData
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


/** Perform a tick in the engine
 *
 * This function will check for collision between all objects and
 * call the related collision hook. Afterwards it will update all
 * objects with their new positions.
 *
 * @param time Time since application start
 */
void Physics_checkCollisions(TIME time, BOOL do_update);


void Physics_update(BOOL do_update);


/** Add default handler Hook for solid, gliding objects
 */
void Physics_setDefaultCollisionHandler(BODY_TYPE left, BODY_TYPE right);

#endif /* SMUG_PHYSICS_PHYSICS_H */

/**@}*/
