#include "physics.h"
#include "debug.h"

#include "common/log.h"

#include "utils/linkedlist.h"
#include "utils/vector.h"
#include "utils/map.h"

#include <math.h>
#include <stdlib.h>

/** Type used for the map of collision hooks */
typedef struct _CollisionType
{
    BODY_TYPE self;
    BODY_TYPE other;
} _CollisionType;

/** Map of all added bodies in the physics. Maps BODY_TYPE => LinkedList<Body*> */ 
static Map* body_map;

/** Map of all added collision hooks. Maps _CollisionType => LinkedList<Hook*> */
static Map* collision_hooks;

/** List with CollisionData for each collision that occured the last tick. */
static LinkedList* collision_list;

/** Whether or not the physics is initialized. */
static BOOL isInitialized = FALSE;

/** Whether or not to draw debug frames. */
static BOOL debugMode = FALSE;

/* FUNCTION DECLARATION */
static _CollisionType* _CollisionType_new(void);
static void _CollisionType_delete(_CollisionType* type);
static BOOL _compareCollisionType(void* self, void* other);
static BOOL _collideMovingPoints1D(float x1_start, float x1_end, float x2_start, float x2_end, float *t);
static BOOL _collideMovingInterval1D(float i1_x1_start, float i1_x1_end, float i1_x2_start, float i1_x2_end, 
				     float i2_x1_start, float i2_x1_end, float i2_x2_start, float i2_x2_end, 
				     float* t_in, float* t_out);

static BOOL _collideRectangleRectangle(Body* self, Body* other, CollisionData** collision_data);
static void _detectCollisions(LinkedList* self, LinkedList* other, LinkedList* collisions);
static void _handleCollisions(LinkedList* collisions, LinkedList* hooks);

  
//===========================================
// STATIC FUNCTIONS (PRIVATE)
//===========================================

/** Allocate memory for _CollisionType */
static _CollisionType* _CollisionType_new(void)
{
    return malloc(sizeof(_CollisionType));
}


/** Free memory for _CollisionType */
static void _CollisionType_delete(_CollisionType* type)
{
    free(type);
}


/** Compare two _CollisionTypes, used for Map */
static BOOL _compareCollisionType(void* self, void* other)
{
    if(((_CollisionType*) self)->self == ((_CollisionType*) other)->self &&
       ((_CollisionType*) self)->other == ((_CollisionType*) other)->other)
        {
            return TRUE;
        }

    return FALSE;
}


/** Find the collision time of two moving points in 1 dimension.
 *
 * Given two points on the X-axis, we will find when (if) they collide.
 *
 * @param x1_start First points starting position
 * @param x1_end   First points ending position
 * @param x2_start Second points starting position
 * @param x2_end   Second points ending position
 * @param[out] t   Time for collision
 *
 * @return Whether or not the points collide
 */
static BOOL _collideMovingPoints1D(float x1_start, float x1_end, float x2_start, float x2_end, float *t)
{
    float denumerator, numerator;

    // Simply solve the equation: x1_start + t*(x1_end - x1_start) = x2_start + t*(x2_end - x1_start)
    numerator = x1_start - x2_start;
    denumerator = x2_end - x1_end + x1_start - x2_start;

    // If the denumerator is 0, the points are either not moving or
    // moving with the same speed
    if(denumerator == 0) {
        return FALSE;
    }

    // Calculate the time and return True
    *t = numerator / denumerator;

    return TRUE;
}


/** Find the collision time for two moving intervals in 1 dimension.
 *
 * Given two intervals on the X-axis, we will find when (if) they collide. 
 * This is done by comparing the collision times of the moving ending points
 * for each interval.
 *
 * The moving intervals can be interpreted as:
 *
 * 0s: [i1_x1_start]----[i1_x2_start]
 * 0s:                                    [i2_x1_start]-----------[i2_x2_start]
 *
 * 1s:         [i1_x1_end]----[i1_x2_end]
 * 1s:                         [i2_x1_end]-----------[i2_x2_end]
 *
 * @relatesalso _collideMovingPoints1D
 *
 * @param i1_x1_start  First intervals left starting point
 * @param i1_x1_end    First intervals left ending point
 * @param i1_x2_start  First intervals right starting point
 * @param i1_x2_end    First intervals right ending point
 * @param i2_x1_start  Second intervals left starting point
 * @param i2_x1_end    Second intervals left ending point
 * @param i2_x2_start  Second intervals right starting point
 * @param i2_x2_end    Second intervals right ending point
 * @param[out] t_in    Starting time for interval overlap
 * @param[out] t_out   Ending time for interval overlap
 *
 * @return Whether or not the intervals will overlap
 */
static BOOL _collideMovingInterval1D(float i1_x1_start, float i1_x1_end, float i1_x2_start, float i1_x2_end,
                               float i2_x1_start, float i2_x1_end, float i2_x2_start, float i2_x2_end,
                               float* t_in, float* t_out)
{
    float x1_in = 0;
    float x1_out = 0;
    float x2_in = 0;
    float x2_out = 0;

    BOOL res = TRUE;

    // Check the first intervals left point against the second intervals left point
    res *= (1 - _collideMovingPoints1D(i1_x1_start, i1_x1_end, i2_x1_start, i2_x1_end, &x1_in));

    // Check the first intervals left point against the second intervals right point
    res *= (1 - _collideMovingPoints1D(i1_x1_start, i1_x1_end, i2_x2_start, i2_x2_end, &x1_out));

    // Check the first intervals right point against the second intervals left point
    res *= (1 - _collideMovingPoints1D(i1_x2_start, i1_x2_end, i2_x1_start, i2_x1_end, &x2_in));

    // Check the first intervals right point against the second intervals right point
    res *= (1 - _collideMovingPoints1D(i1_x2_start, i1_x2_end, i2_x2_start, i2_x2_end, &x2_out));

    // If no collisions were found, check for start
    if(TRUE == res)
        {
            // Check if the intervals started as non-overlapping
            if(max(i1_x1_start, i1_x2_start) <= min(i2_x1_start, i2_x2_start) ||
               max(i2_x1_start, i2_x2_start) <= min(i1_x1_start, i1_x2_start))
                {
                    return FALSE;
                }
	    // Otherwise, we started as overlapping which means we are in collision. However,
	    // we can't say anything about when the collision occured.
            else
                {
                    *t_in = -100000.0;
                    *t_out = 100000.0;
                    return TRUE;
                }
        }

    // Fix so the _in variables are lesser than the _out variables
    if(x1_in > x1_out) swap_float(&x1_in, &x1_out);
    if(x2_in > x2_out) swap_float(&x2_in, &x2_out);

    // Set t_in to smallest _in value
    *t_in = x1_in < x2_in ? x1_in : x2_in;

    // Set t_out to the greates _out value
    *t_out = x1_out > x2_out ? x1_out : x2_out;

    return TRUE;
}


/** Find collision for two moving rectangles
 *
 * This will find the collision time for two moving rectangles. It 
 * is done by comparing both rectangles projections in the X and Y
 * plane respectively, and see if these projections collide at the
 * same time.
 *
 * Since it is rectangles, the projections will simply be the interval
 * from its left/top x-/y-point to its right/bottom x-/y-point.
 */
static BOOL _collideRectangleRectangle(Body* self, Body* other, CollisionData** collision_data)
{
    float t_x_in = 0;
    float t_x_out = 0;
    float t_y_in = 0;
    float t_y_out = 0;

    float self_x_start = Rectangle_getX(self->shape->data) + Point_getX(&self->position);
    float self_x_end = Rectangle_getX(self->shape->data) + Point_getX(&self->new_position);
    float self_width = Rectangle_getW(self->shape->data);

    float other_x_start = Rectangle_getX(other->shape->data) + Point_getX(&other->position);
    float other_x_end = Rectangle_getX(other->shape->data) + Point_getX(&other->new_position);
    float other_width = Rectangle_getW(other->shape->data);

    float self_y_start = Rectangle_getY(self->shape->data) + Point_getY(&self->position);
    float self_y_end = Rectangle_getY(self->shape->data) + Point_getY(&self->new_position);
    float self_height = Rectangle_getH(self->shape->data);

    float other_y_start = Rectangle_getY(other->shape->data) + Point_getY(&other->position);
    float other_y_end = Rectangle_getY(other->shape->data) + Point_getY(&other->new_position);
    float other_height = Rectangle_getH(other->shape->data);

    BOOL res = TRUE;

    // Check for collision of the moving X-projections of the rectangle
    res *= (1 - _collideMovingInterval1D(self_x_start, self_x_end, self_x_start+self_width, self_x_end+self_width,
					 other_x_start, other_x_end, other_x_start+other_width, other_x_end+other_width,
					 &t_x_in, &t_x_out));

    // Check for collision of the moving Y-projections of the rectangle
    res *= (1 - _collideMovingInterval1D(self_y_start, self_y_end, self_y_start+self_height, self_y_end+self_height,
					 other_y_start, other_y_end, other_y_start+other_height, other_y_end+other_height,
					 &t_y_in, &t_y_out));

    // If none of the intervals collide, the rectangles does not collide
    if(TRUE == res)
        {
            return FALSE;
        }

    // If the intervals collide, but not both at the same time, the rectangles
    // does not collide.
    if(t_x_out < t_y_in || t_y_out < t_x_in)
        {
            return FALSE;
        }

    // If the collision occurs within the time 0.0 and 1.0 we have a collision.
    // Otherwise the collision will be in the past (t < 0.0) or in a future 
    // tick (t > 1.0).
    if(t_x_in < 1.0 && t_y_in < 1.0 && t_x_out > 0.0 && t_y_out > 0.0) {
        (*collision_data) = CollisionData_new();
        (*collision_data)->self = self;
        (*collision_data)->other = other;
	
	// If the X-projection collides before the Y-projection, use t_x_in and
	// set the normal to Vector(x=1,y=0) or Vector(x=-1,y=0).
	if(t_x_in > t_y_in) {
	  (*collision_data)->collisionTime =  t_x_in;
	  (*collision_data)->contactNormal = Vector_create2d( (self_x_start - other_x_start) / abs(self_x_start - other_x_start), 0.0f);
	} 
	// If the Y-projection collides before the X-projection, use t_y_in and
	// set the normal to Vector(x=0,y=1) or Vector(x=0,y=-1).
	else {
	  (*collision_data)->collisionTime =  t_y_in;
	  (*collision_data)->contactNormal = Vector_create2d(0.0f, (self_y_start - other_y_start) / abs(self_y_start - other_y_start));
	}
        	
	(*collision_data)->selfMovement = Point_distanceToPoint(self->position, self->new_position);

        return TRUE;
    }

    // The collision is in the past or the future, return FALSE.
    return FALSE;
}


static void _detectCollisions(LinkedList* self, LinkedList* other, LinkedList* collisions)
{
    CollisionData* collision_data;
    Vector result;
    Node* self_node;
    Node* other_node;

    if(NULL == self->first || NULL == other->first)
        {
            return;
        }

    for(self_node = self->first; self_node != NULL; self_node = self_node->next)
        {
            for(other_node = other->first; other_node != NULL; other_node = other_node->next)
                {
                    if(other_node->item == self_node->item)
                        {
                            continue;
                        }

                    Body* self_body = (Body*) self_node->item;
                    Body* other_body = (Body*) other_node->item;

                    if(NULL == self_body->shape || NULL == other_body->shape)
                        {
                            continue;
                        }

                    if(SHAPE_RECTANGLE == self_body->shape->type &&
                       SHAPE_RECTANGLE == other_body->shape->type)
                        {
                            if(TRUE == _collideRectangleRectangle(self_body, other_body, &collision_data))
                                {
                                    LinkedList_addLast(collisions, collision_data);
                                }
                        }
                }
        }
}


/** Iterate over the collision list and call matching hooks. */
static void _handleCollisions(LinkedList* collisions, LinkedList* hooks)
{
    Node* node;

    for(node = collisions->first; node != NULL; node = node->next)
        {
            CollisionData* data = (CollisionData*) node->item;

            Hook_callAll(hooks, data);
        }
}


//===========================================
// PUBLIC FUNCTIONS
//===========================================

CollisionData* CollisionData_new(void)
{
    return malloc(sizeof(CollisionData));
}


void CollisionData_delete(CollisionData* data)
{
    free(data);
}


int Physics_init(void)
{
    assert(!isInitialized);

    body_map = Map_new();
    collision_hooks = Map_new();
    collision_hooks->compareKeys = _compareCollisionType;
    collision_list = LinkedList_new();

    DEBUG("Initializing physics");
    isInitialized = TRUE;
    return 1;
}


BOOL Physics_isInitialized(void)
{
    return isInitialized;
}


void Physics_terminate(void)
{
    assert(isInitialized);
    MapNode* node;

    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList_delete((LinkedList*) node->value);
        }

    LinkedList_delete(collision_list);
    Map_delete(body_map);
    Map_delete(collision_hooks);
    isInitialized = FALSE;
}


void Physics_addCollisionHook(BODY_TYPE self, BODY_TYPE other, Hook* hook)
{
    _CollisionType type;

    type.self = self;
    type.other = other;

    LinkedList* list = Map_get(collision_hooks, (void*) &type);

    if(NULL == list)
        {
            _CollisionType* _type = _CollisionType_new();
            _type->self = self;
            _type->other = other;
            list = LinkedList_new();

            Map_set(collision_hooks, (void*) _type, list);
        }

    LinkedList_addLast(list, (void*) hook);
}


void Physics_addBody(Body* body)
{
    LinkedList* list = Map_get(body_map, (void*) body->type);
    DEBUG("Adding body");

    if(NULL == list)
        {
            list = LinkedList_new();
            Map_set(body_map, (void*) body->type, list);
        }

    LinkedList_addLast(list, body);
}


void Physics_removeBody(Body* body)
{
    LinkedList* list = Map_get(body_map, (void*) body->type);

    if(NULL != list)
        {
            LinkedList_removeItem(list, body);
        }
}


void Physics_update(TIME time)
{
    MapNode* node;
    Node* list_node;

    // Clean up collision data from last iteration
    for(list_node = collision_list->last; list_node != NULL; list_node = list_node->prev)
        {
            CollisionData_delete(list_node->item);
            LinkedList_remove(collision_list, list_node);
        }

    // Update all objects new_position with the value from their movement.
    // This does not update their actual positions, but the position to 
    // where they want to move.
    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList* list;
            Node* list_node;

            list = (LinkedList*) node->value;

            for(list_node = list->first; list_node != NULL; list_node = list_node->next)
                {
                    Body* body = list_node->item;

                    body->new_position = Point_addVector(body->new_position, body->movement);
                }
        }

    // Iterate over all objects with an associated CollisionHook and detect
    // their collision. Without hooks their will be no handling of the collision
    // and thus no need to detect them.
    for(node = collision_hooks->first; node != NULL; node = node->next)
        {
            LinkedList* self;
            LinkedList* other;

            self = Map_get(body_map, ((_CollisionType *) node->key)->self);
            other = Map_get(body_map, ((_CollisionType *) node->key)->other);

            if(NULL != self && NULL != other)
                {
                    _detectCollisions(self, other, collision_list);
                }
        }

    // DEBUG - DEBUG - DEBUG - DEBUG
    // Draw the shapes and the position to where they want to move.
    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList* list;
            Node* list_node;

            list = (LinkedList*) node->value;

            for(list_node = list->first; list_node != NULL; list_node = list_node->next)
                {
                    Body* body = list_node->item;

                    Physics_drawShape(body->shape, body->position, Color_createFromRGBA(1.0, 0.0, 0.0, 1.0));
                    Physics_drawLine(body->position, body->new_position, Color_createFromRGBA(0.0,1.0,0.0,1.0));
                    Physics_drawShape(body->shape, body->new_position, Color_createFromRGBA(0.0, 1.0, 0.0, 1.0));
                }
        }

    // Call all collision hooks and the found collisions for each type
    for(node = collision_hooks->first; node != NULL; node = node->next)
        {
            _handleCollisions(collision_list, (LinkedList*) node->value);
        }

    // Update all bodies with their handled new_positions.
    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList* list;
            Node* list_node;

            list = (LinkedList*) node->value;

            for(list_node = list->first; list_node != NULL; list_node = list_node->next)
                {
                    Body* body = list_node->item;

                    body->position = body->new_position;
                }
        }

}
