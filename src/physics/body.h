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

/** Physical representation of an object
 *
 *
 */
typedef struct Body
{
    BODY_TYPE type;
    Shape* shape;
    Point position;
    Point new_position;
    Vector movement;
} Body;


/** Create a new Body object
 * @relatesalso Body
 */
Body* Body_new(void);


/** Free memory used by a Body object
 * @relatesalso Body
 * @param body The Body to free
 */
void Body_delete(Body* body);


/** Set the shape of a Body object
 *
 * This function will set the Shape to use for the
 * object.
 *
 * @relatesalso Body
 * @param body The Body to change
 * @param shape The Shape to set
 */
void Body_setShape(Body* body, Shape* shape);

/** Set the Bodys position wihtout collision handling
 *
 * @relatesalso Body
 */
void Body_setPosition(Body* body, float x, float y);

/** Move the Body in the given direction
 *
 * @relatesalso Body
 */
void Body_move(Body* body, float x, float y);

/** Move the Body to the given position
 *
 * @relatesalso Body
 */
void Body_moveTo(Body* body, float x, float y);

#endif // SMUG_PHYSICS_BODY_H

/**@}*/
