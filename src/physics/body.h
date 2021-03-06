/**
 * @file body.h
 * @brief
 * @if doc_files
 * @ingroup smug_physics
 * @endif
 */

/**
 * @addtogroup smug_physics
 * @{
 */

#ifndef SMUG_PHYSICS_BODY_H
#define SMUG_PHYSICS_BODY_H

#include <utils/shape.h>
#include <physics/waypoint.h>
#include <utils/linkedlist.h>
#include <graphics/color_type.h>

typedef unsigned int BODY_TYPE;

/** Physical representation of an object
 *
 *
 */
typedef struct Body
{
    BODY_TYPE type;
    BOOL immovable;
    Shape* shape;
    Point position;
    Vector velocity;
    Vector acceleration;
    float friction;
    float elasticity;
    float mass;

    LinkedList* waypoints;

    Vector immovableNormal;
    float  immovableWeight;

    Vector _newVelocity;
} Body;


/** Create a new Body object
 * @relates Body
 */
Body* Body_new(void);


/** Free memory used by a Body object
 * @relates Body
 * @param body The Body to free
 */
void Body_delete(Body* body);


/** Set the shape of a Body object
 *
 * This function will set the Shape to use for the
 * object.
 *
 * @relates Body
 * @param body The Body to change
 * @param shape The Shape to set
 */
void Body_setShape(Body* body, Shape* shape);

/** Set the Bodys position wihtout collision handling
 *
 * @relates Body
 */
void Body_setPosition(Body* body, Point position);

/** Set the Bodys velocity
 *
 * @relates Body
 */
void Body_setVelocity(Body* body, Vector velocity);

/** Move the Body in the given direction
 *
 * @relates Body
 */
void Body_move(Body* body, float x, float y);

/** Move the Body to the given position
 *
 * @relates Body
 */
void Body_moveTo(Body* body, float x, float y);

void Body_clearWaypoints(Body* body);
void Body_addWaypoint(Body* body, Waypoint* waypoint);
void Body_removeWaypointsAfterTime(Body* body, float time);

void Body_drawBody(Body* body, float time, Color color);
void Body_dump(Body* body);
void Body_dumpWaypoints(Body* body);
#endif // SMUG_PHYSICS_BODY_H

/**@}*/
