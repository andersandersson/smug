/** @file shapes.h
  * @brief
  * @ingroup smug_physics
  */

/** @addtogroup smug_physics
  * @{
  */

#ifndef SMUG_PHYSICS_BODY_H
#define SMUG_PHYSICS_BODY_H

#include "shapes.h"

typedef unsigned int BODY_TYPE;

typedef struct Body
{
    BODY_TYPE type;
    Shape* shape;
    Point position;
    Point new_position;
    Vector movement;
} Body;

/**
 * @relatesalso Body
 */
Body* Body_new(void);

/**
 * @relatesalso Body
 */
void Body_delete(Body* body);

/**
 * @relatesalso Body
 */
void Body_setShape(Body* body, Shape* shape);

/**
 * @relatesalso Body
 */
void Body_setPosition(Body* body, float x, float y);

/**
 * @relatesalso Body
 */
void Body_move(Body* body, float x, float y);

/**
 * @relatesalso Body
 */
void Body_moveTo(Body* body, float x, float y);

#endif // SMUG_PHYSICS_BODY_H

/**@}*/
