#include <math.h>
#include <stdlib.h>
#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <graphics/color.h>
#include <utils/linkedlist.h>
#include <utils/vector.h>
#include <utils/rectangle.h>
#include <utils/point.h>
#include <utils/orderedset.h>
#include <utils/map.h>
#include <physics/debug.h>
#include <physics/physics.h>
#include <physics/calc.h>
#include <platform/platform.h>

// Position epsilon
#define P_EPSILON 0.01

#define static

Body* current_body = NULL;

/** Type used for the map of collision hooks */
typedef struct _CollisionHookType
{
    BODY_TYPE self;
    BODY_TYPE other;
} _CollisionHookType;

/** Map of all added bodies in the physics. Maps BODY_TYPE => LinkedList<Body*> */
static Map* body_map;

/** Map of all added collision hooks. Maps _CollisionHookType => (COLLISION_TYPE => LinkedList<Hook*> */
static Map* collision_hooks[COLLISION_TYPE_LENGTH];

/** Set with different hooktypes */
static OrderedSet* collision_types;

/** Whether or not the physics is initialized. */
static BOOL isInitialized = FALSE;

/** Whether or not to draw debug frames. */
static BOOL debugMode = FALSE;

/* FUNCTION DECLARATION */
static _CollisionHookType* _CollisionHookType_new(void);
static void _CollisionHookType_delete(_CollisionHookType* type);
static int _compareCollisionHookType(void* self, void* left, void* right);
static int _compareCollisionData(void* self, void* left, void* right);
static BOOL _isBodyInvolved(void* self, void* other);

int _default_handleEnterCollision(void* lparam, void* rparam);
int _default_handleInCollision(void* lparam, void* rparam);
int _default_handleExitCollision(void* lparam, void* rparam);


//===========================================
// STATIC FUNCTIONS (PRIVATE)
//===========================================

/** Allocate memory for _CollisionType */
static _CollisionHookType* _CollisionHookType_new(void)
{
    return malloc(sizeof(_CollisionHookType));
}

/** Free memory for _CollisionType */
static void _CollisionHookType_delete(_CollisionHookType* type)
{
    free(type);
}

/** Compare two _CollisionTypes, used for Map */
static int _compareCollisionHookType(void* self, void* left, void* right)
{
    _CollisionHookType* _self = (_CollisionHookType*) left;
    _CollisionHookType* _other = (_CollisionHookType*) right;

    if(_self->self == _other->self && _self->other == _other->other ||
       _self->self == _other->other && _self->other == _other->self)
    {
	return 0;
    }

    if(_self->self < _other->self)
    {
	return -1;
    }
    else if(_self->self > _other->self)
    {
	return 1;
    }

    if(_self->other < _other->other)
    {
	return -1;
    }
    else if(_self->other > _other->other)
    {
	return 1;
    }

    return 0;
}


/** Compare two _CollisionKeys, used for Map */
static int _compareCollisionData(void* self, void* left, void* right)
{
    CollisionData* _self = (CollisionData*) left;
    CollisionData* _other = (CollisionData*) right;

    // Always let the same object equal itself
    if(_self == _other)
    {
	return 0;
    }

    // If the pair is already in the collision list, consider them to be the same
    if(((_self->body[0] == _other->body[0] && _self->body[1] == _other->body[1]) ||
        (_self->body[0] == _other->body[1] && _self->body[1] == _other->body[0])) &&
       _self->type == _other->type)
    {
	return 0;
    }

    if(_self->absoluteTime < _other->absoluteTime)
    {
	return -1;
    }

    if(_self->absoluteTime > _other->absoluteTime)
    {
	return 1;
    }

    if(_self->type < _other->type)
    {
        return -1;
    }

    if(_self->type > _other->type)
    {
        return 1;
    }

    if(_self->body[0] < _other->body[0])
    {
        return -1;
    }

    if(_self->body[0] > _other->body[0])
    {
        return 1;
    }

    if(_self->body[1] < _other->body[1])
    {
        return -1;
    }

    if(_self->body[1] > _other->body[1])
    {
        return 1;
    }

    // What should this return as default value?
    return 0;
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
static BOOL _collideRectangleRectangle(Body* self, Point* self_start, Vector* self_velocity, Vector* self_acceleration,
				       Body* other, Point* other_start, Vector* other_velocity, Vector* other_acceleration,
				       TIME delta_time, LinkedList* collisions)
{
    CollisionData* collision_data;

    float self_x_start = Rectangle_getX(self->shape->data) + Point_getX(*self_start);
    float self_x_vel = Vector_getX(self_velocity);
    float self_x_acc = Vector_getX(self_acceleration);
    float self_width = Rectangle_getW(self->shape->data);

    float other_x_start = Rectangle_getX(other->shape->data) + Point_getX(*other_start);
    float other_x_vel = Vector_getX(other_velocity);
    float other_x_acc = Vector_getX(other_acceleration);
    float other_width = Rectangle_getW(other->shape->data);

    float self_y_start = Rectangle_getY(self->shape->data) + Point_getY(*self_start);
    float self_y_vel = Vector_getY(self_velocity);
    float self_y_acc = Vector_getY(self_acceleration);
    float self_height = Rectangle_getH(self->shape->data);

    float other_y_start = Rectangle_getY(other->shape->data) + Point_getY(*other_start);
    float other_y_vel = Vector_getY(other_velocity);
    float other_y_acc = Vector_getY(other_acceleration);
    float other_height = Rectangle_getH(other->shape->data);

    float t_x_in[2], t_x_out[2];
    int x_p = 0;

    float t_y_in[2], t_y_out[2];
    int y_p = 0;

    // Check for collision of the moving X-projections of the rectangle
    x_p = _collideMovingInterval1D(self_x_start, self_x_vel, self_x_acc, self_width,
				   other_x_start, other_x_vel, other_x_acc, other_width,
				   &t_x_in[0], &t_x_out[0], &t_x_in[1], &t_x_out[1]);

    y_p = _collideMovingInterval1D(self_y_start, self_y_vel, self_y_acc, self_height,
				   other_y_start, other_y_vel, other_y_acc, other_height,
				   &t_y_in[0], &t_y_out[0], &t_y_in[1], &t_y_out[1]);

    float wayout_x;
    float wayout_y;

    BOOL overlap_x = FALSE;
    BOOL overlap_y = FALSE;
    BOOL overlap_start = FALSE;

    BOOL same_x_speed = FALSE;
    BOOL same_y_speed = FALSE;

    if(self_x_vel == other_x_vel)
    {
        same_x_speed = TRUE;
    }

    if(self_y_vel == other_y_vel)
    {
        same_y_speed = TRUE;
    }

    int side;

    side = _collideInterval1D(self_x_start, self_width, other_x_start, other_width, &wayout_x);
    if(wayout_x > P_EPSILON)
    {
	overlap_x = TRUE;

	if(side == -1 || (side == 0 && self < other))
        {
	    wayout_x *= -1.0;
        }
    }

    side = _collideInterval1D(self_y_start, self_height, other_y_start, other_height, &wayout_y);
    if(wayout_y > P_EPSILON)
    {
	overlap_y = TRUE;

	if(side == -1 || (side == 0 && self < other))
        {
	    wayout_y *= -1.0;
        }
    }

    if(0 == x_p && TRUE == overlap_x)
    {
	x_p = 1;
	t_x_in[0] = -FP_INFINITE;
	t_x_out[0] = FP_INFINITE;
    }

    if(0 == y_p && TRUE == overlap_y)
    {
	y_p = 1;
	t_y_in[0] = -FP_INFINITE;
	t_y_out[0] = FP_INFINITE;
    }

    overlap_start = overlap_x * overlap_y;

    if(TRUE == overlap_start)
    {
	collision_data = CollisionData_new();
	collision_data->body[0] = self;
	collision_data->body[1] = other;

	if(fabs(wayout_x) > fabs(wayout_y))
        {
	    collision_data->wayout[0] = Vector_create2d( 0.0, wayout_y );
	    collision_data->wayout[1] = Vector_create2d( 0.0, -wayout_y );
        }
	else
        {
	    collision_data->wayout[0] = Vector_create2d( wayout_x, 0.0 );
	    collision_data->wayout[1] = Vector_create2d( -wayout_x, 0.0 );
        }

	collision_data->time = 0.0;
	collision_data->type = COLLISION_PERSIST;

	LinkedList_addLast(collisions, collision_data);
    }

    int i;
    int j;

    for(i=0; i<x_p; i++)
        for(j=0; j<y_p; j++)
	{
            float t_in = 0.0;
            float t_out = delta_time;
            float t_mid = 0.0;

            t_in = max(t_x_in[i], t_y_in[j]);

            t_out = min(t_x_out[i], t_y_out[j]);

            t_mid = (t_in + t_out)*0.5;

            if(FALSE == overlap_start && t_in < delta_time && t_mid > 0.0 && t_in < t_out)
	    {
                Point end_point = Point_addVector(*self_start, Vector_add(Vector_multiply(*self_velocity, t_in),
                                                                          Vector_multiply(*self_acceleration, t_in*t_in*0.5)));

                Point dest_point = Point_addVector(*self_start, Vector_add(Vector_multiply(*self_velocity, delta_time),
                                                                           Vector_multiply(*self_acceleration, delta_time*delta_time*0.5)));

                float distance_left = Vector_length(Point_distanceToPoint(end_point, dest_point));

                if(distance_left > P_EPSILON)
		{
                    collision_data = CollisionData_new();
                    collision_data->body[0] = self;
                    collision_data->body[1] = other;
                    collision_data->type = COLLISION_ENTER;
                    collision_data->time =  t_in;

                    // If X-axis enters before Y-axis
                    if( (t_x_in[i] > t_y_in[j]) && FALSE == same_x_speed) {
                        collision_data->normal[0] = Vector_create2d( (self_x_start - other_x_start) / fabs(self_x_start - other_x_start), 0.0f);
                        collision_data->normal[1] = Vector_create2d( -1.0f * (self_x_start - other_x_start) / fabs(self_x_start - other_x_start), 0.0f);
                    } else {
                        collision_data->normal[0] = Vector_create2d( 0.0f, (self_y_start - other_y_start) / fabs(self_y_start - other_y_start));
                        collision_data->normal[1] = Vector_create2d( 0.0f, -1.0f * (self_y_start - other_y_start) / fabs(self_y_start - other_y_start));
                    }

                    LinkedList_addLast(collisions, collision_data);
                    if(! (collision_data->normal[0].d[0] > -640.0 && collision_data->normal[0].d[0] < 640.0 && collision_data->normal[0].d[1] > -480.0 && collision_data->normal[0].d[1] < 480.0) )
                    {
                        DEBUG("Oh oh 1");
                    }
		}
	    }

            if(t_out < delta_time && t_out > 0.0)
	    {
                collision_data = CollisionData_new();
                collision_data->body[0] = self;
                collision_data->body[1] = other;
                collision_data->type = COLLISION_EXIT;
                collision_data->time =  t_out;

                if(t_x_out[i] < t_y_out[j]) {
                    collision_data->normal[0] = Vector_create2d( wayout_x / fabs(wayout_x), 0.0f);
                    collision_data->normal[1] = Vector_create2d( -wayout_x / fabs(wayout_x), 0.0f);
                } else {
                    collision_data->normal[0] = Vector_create2d( 0.0f, wayout_y / fabs(wayout_y));
                    collision_data->normal[1] = Vector_create2d( 0.0f, -wayout_y / fabs(wayout_y));
                 }

                LinkedList_addLast(collisions, collision_data);
                if(! (collision_data->normal[0].d[0] > -640.0 && collision_data->normal[0].d[0] < 640.0 && collision_data->normal[0].d[1] > -480.0 && collision_data->normal[0].d[1] < 480.0) )
                {
                    DEBUG("Oh oh 2");
                }
	    }
	}
    // TODO: What should this function return?

    return TRUE;
}


static BOOL _detectCollisions(LinkedList* self, LinkedList* other, TIME delta_time, OrderedSet* collisions)
{
    LinkedList* new_collisions = LinkedList_new();

    Node* self_node;
    Node* other_node;

    // Point self_start;
    // Point self_end;
    // Point other_start;
    // Point other_end;

    BOOL changes_made = FALSE;

    if(NULL == self->first || NULL == other->first)
    {
        return FALSE;
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

            Node* self_left_wp_node = self_body->waypoints->first;
            Node* self_right_wp_node = self_body->waypoints->first->next;

            Node* other_left_wp_node = other_body->waypoints->first;
            Node* other_right_wp_node = other_body->waypoints->first->next;

            while(self_right_wp_node != NULL && other_right_wp_node != NULL)
            {
                Waypoint* self_left_wp = (Waypoint*) self_left_wp_node->item;
                Waypoint* self_right_wp = (Waypoint*) self_right_wp_node->item;
                Waypoint* other_left_wp = (Waypoint*) other_left_wp_node->item;
                Waypoint* other_right_wp = (Waypoint*) other_right_wp_node->item;

                float t_start = max(self_left_wp->time, other_left_wp->time);
                float t_end = min(self_right_wp->time, other_right_wp->time);

                if(t_start != t_end)
                {
                    float self_delta_time = t_start - self_left_wp->time;
                    float other_delta_time = t_start - other_left_wp->time;

                    Point self_start = Point_addVector(self_left_wp->point, Vector_add(Vector_multiply(self_left_wp->velocity, self_delta_time),
                                                                                       Vector_multiply(self_left_wp->acceleration, self_delta_time*self_delta_time*0.5)));
                    Point other_start = Point_addVector(other_left_wp->point, Vector_add(Vector_multiply(other_left_wp->velocity, other_delta_time),
                                                                                         Vector_multiply(other_left_wp->acceleration, other_delta_time*other_delta_time*0.5)));

                    if(SHAPE_RECTANGLE == self_body->shape->type &&
                       SHAPE_RECTANGLE == other_body->shape->type)
                    {
                        if(TRUE == _collideRectangleRectangle(self_body, &self_start, &self_left_wp->velocity, &self_left_wp->acceleration,
                                                              other_body, &other_start, &other_left_wp->velocity, &other_left_wp->acceleration,
                                                              t_end - t_start, new_collisions))
                        {
                            changes_made = TRUE;

                            while(new_collisions->first != NULL)
                            {
                                CollisionData* data = (CollisionData*) new_collisions->first->item;
                                float dt = data->time;

                                data->start[0] = self_start;
                                data->start[1] = other_start;

                                data->movement[0] = Vector_add(Vector_multiply(self_left_wp->velocity, dt), Vector_multiply(self_body->acceleration, dt*dt*0.5));
                                data->movement[1] = Vector_add(Vector_multiply(other_left_wp->velocity, dt), Vector_multiply(other_body->acceleration, dt*dt*0.5));

                                data->velocity[0] = self_left_wp->velocity;
                                data->velocity[1] = other_left_wp->velocity;

                                data->acceleration[0] = self_left_wp->acceleration;
                                data->acceleration[1] = other_left_wp->acceleration;

                                data->absoluteTime = t_start + data->time;
                                data->absoluteTimeStart = t_start;
                                data->deltaTime = delta_time;

                                OrderedSet_insert(collisions, data);

                                LinkedList_removeItem(new_collisions, (void*) data);
                            } // while
                        } // if (TRUE == _collide)
                    } // if (SHAPE_RECTANGLE)
                } // if (t_start != t_end)

                if(self_right_wp->time < other_right_wp->time)
                {
                    self_left_wp_node = self_right_wp_node;
                    self_right_wp_node = self_right_wp_node->next;
                }
                else
                {
                    other_left_wp_node = other_right_wp_node;
                    other_right_wp_node = other_right_wp_node->next;
                }
            } // while
        } // for
    } // for

    LinkedList_delete(new_collisions);

    return changes_made;
}


int _default_handleEnterCollision(void* lparam, void* rparam)
{
    CollisionData* data = (CollisionData*) rparam;

    int result = 0;

    int i = 0;
    for(i = 0; i<2; i++)
    {
        int self = i % 2;
        int other = (i+1) % 2;

        if(data->body[self]->immovable == TRUE)
        {
            continue;
        }

        Vector movement = data->movement[self];

        float m1 = data->body[self]->mass;
        float m2 = data->body[other]->mass;

        Vector self_velocity = Vector_add(data->velocity[self], Vector_multiply(data->acceleration[self], data->absoluteTime - data->absoluteTimeStart));
        Vector other_velocity = Vector_add(data->velocity[other], Vector_multiply(data->acceleration[other], data->absoluteTime - data->absoluteTimeStart));
        Vector self_acceleration = data->acceleration[self];
        Point self_start = data->start[self];

        Vector velocity_self_n = Vector_projection(self_velocity, data->normal[self]);
        Vector velocity_self_p = Vector_sub(self_velocity, velocity_self_n);

        float v1 = Vector_dotProduct(self_velocity, data->normal[self]);
        float v2 = Vector_dotProduct(other_velocity, data->normal[self]);

        float new_v1;

        if(data->body[other]->immovable == TRUE)
        {
            new_v1 = -1.0 * v1 + 2 * v2;
        }
        else
        {
            new_v1 = (m1 - m2) / (m1 + m2) * v1 + (2 * m2) / (m1 + m2) * v2;
        }

        if(fabs(new_v1) < 10)
        {
            new_v1 = 0.0;
        }

        Vector new_velocity = Vector_add(velocity_self_p, Vector_multiply(data->normal[self], new_v1*data->body[self]->elasticity));
        Vector next_velocity = Vector_add(new_velocity, Vector_multiply(self_acceleration, data->deltaTime - data->absoluteTime));
        Point new_position = Point_addVector(self_start, movement);
        Point next_position = Point_addVector(new_position, Vector_add(
                                                  Vector_multiply(new_velocity, data->deltaTime - data->absoluteTime),
                                                  Vector_multiply(self_acceleration, (data->deltaTime - data->absoluteTime)*(data->deltaTime - data->absoluteTime)*0.5)));

        float new_length = Vector_length(new_velocity);

        Body_removeWaypointsAfterTime(data->body[self], data->absoluteTime);

        if(fabs(new_v1) >= 4)
        {
            Body_addWaypoint(data->body[self], Waypoint_newFromTPVA(data->absoluteTime, new_position, new_velocity, data->acceleration[self]));
            Body_addWaypoint(data->body[self], Waypoint_newFromTPVA(data->deltaTime, next_position, next_velocity, data->acceleration[self]));
        }
        else
        {
            Vector new_acceleration = Vector_projection(data->acceleration[self], data->normal[self]);
            new_acceleration = Vector_sub(data->acceleration[self], new_acceleration);

            next_position = Point_addVector(new_position, Vector_add(
                                                Vector_multiply(new_velocity, data->deltaTime - data->absoluteTime),
                                                Vector_multiply(new_acceleration, (data->deltaTime - data->absoluteTime)*(data->deltaTime - data->absoluteTime)*0.5)));

            Body_addWaypoint(data->body[self], Waypoint_newFromTPVA(data->absoluteTime, new_position, new_velocity, new_acceleration));
            Body_addWaypoint(data->body[self], Waypoint_newFromTPVA(data->deltaTime, next_position, next_velocity, new_acceleration));
        }

        result |= (i+1);
    }

    return result;
}

int _default_handleInCollision(void* lparam, void* rparam)
{
    CollisionData* data = (CollisionData*) rparam;
    Node* node;

    int result = 0;
    DEBUG("======================================");
    int i = 0;
    for(i = 0; i<2; i++)
    {
        int self = i % 2;
        int other = (i+1) % 2;

        Vector wayout = data->wayout[self];

        if(Vector_length(wayout) < P_EPSILON)
        {
            continue;
        }

        if(data->body[self]->immovable == TRUE)
        {
            continue;
        }

        if(data->body[other]->immovable == FALSE)
        {
            Vector_print(wayout);
            wayout = Vector_multiply(wayout, 0.5);
            wayout = Vector_sub(wayout, Vector_projection(data->wayout[self], data->body[self]->immovableNormal));
            wayout = Vector_add(wayout, Vector_projection(data->wayout[self], data->body[other]->immovableNormal));

            Vector_print(wayout);

            if(Vector_length(data->body[other]->immovableNormal) > Vector_length(data->body[self]->immovableNormal))
            {
                data->body[self]->immovableNormal = data->body[other]->immovableNormal;
            }
        }
        else
        {
            data->body[self]->immovableNormal = Vector_normalize(wayout);
        }

        Body_removeWaypointsAfterTime(data->body[self], data->absoluteTime);

        Point new_position = Point_addVector(data->start[self], wayout);
        Vector new_velocity = data->velocity[self];

        Point next_position = calculate_position(new_position, data->velocity[self], data->acceleration[self], (data->deltaTime - data->absoluteTime));
        Vector next_velocity = calculate_velocity(data->velocity[self], data->acceleration[self], (data->deltaTime - data->absoluteTime));

        Body_addWaypoint(data->body[self], Waypoint_newFromTPVA(data->absoluteTime, new_position, new_velocity, data->acceleration[self]));
        Body_addWaypoint(data->body[self], Waypoint_newFromTPVA(data->deltaTime, next_position, next_velocity, data->acceleration[self]));

        result |= (i+1);
    }

    return result;
}

int _default_handleExitCollision(void* lparam, void* rparam)
{
    // CollisionData* data = (CollisionData*) rparam;
    //DEBUG("COLLISION EXIT: %f", data->time);
    return FALSE;
}

static BOOL _isBodyInvolved(void* self, void* data)
{
    CollisionData* _self = (CollisionData*) self;
    CollisionData* _data = (CollisionData*) data;

    // If they have any in common
    if(_self->body[0] == _data->body[0] ||
       _self->body[0] == _data->body[1] ||
       _self->body[1] == _data->body[0] ||
       _self->body[1] == _data->body[1])
    {
        if(_self->absoluteTime < _data->absoluteTime)
        {
            return TRUE;
        }
    }

    return FALSE;
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
    smug_assert(!isInitialized);

    collision_types = OrderedSet_new();
    OrderedSet_setCompare(collision_types, _compareCollisionHookType);

    body_map = Map_new();

    int i;
    for(i=0; i<COLLISION_TYPE_LENGTH; i++)
    {
        collision_hooks[i] = Map_new();
        Map_setCompare(collision_hooks[i], _compareCollisionHookType);
    }

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
    smug_assert(isInitialized);

    OrderedSet_delete(collision_types);

    Map_delete(body_map);

    int i;
    for(i=0; i<COLLISION_TYPE_LENGTH; i++)
    {
        Map_delete(collision_hooks[i]);
    }

    isInitialized = FALSE;
}


void Physics_setCollisionHandler(BODY_TYPE self, BODY_TYPE other, COLLISION_TYPE type, Hook* hook)
{
    _CollisionHookType* _type = _CollisionHookType_new();

    _type->self = self;
    _type->other = other;

    if(FALSE == OrderedSet_insert(collision_types, _type))
    {
        _CollisionHookType_delete(_type);
    }

    LinkedList* hooks = Map_get(collision_hooks[type], _type);
    if(NULL == hooks)
    {
        hooks = LinkedList_new();

        _CollisionHookType* _type = _CollisionHookType_new();

        _type->self = self;
        _type->other = other;

        Map_set(collision_hooks[type], _type, hooks);
   }

    LinkedList_addLast(hooks, hook);
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


void _Physics_drawBodies(Map* bodies, float time, Color color)
{
    MapIterator* iter = MapIterator_new();

    for(MapIterator_reset(bodies, iter); TRUE == MapIterator_valid(iter); MapIterator_step(iter))
    {
        Pair* pair = (Pair*) MapIterator_get(iter);
        LinkedList* list = (LinkedList*) pair->right;
        Node* node;

        for(node = list->first; NULL != node; node = node->next)
	{
            Body* body = (Body*) node->item;

            int cc = ((int)body) % 255;
            float c = (float)cc / 255.0;

            //color.b = c;
            //color.r /= 2.0;

            Node* wp_node;

            for(wp_node = body->waypoints->first; NULL != wp_node; wp_node = wp_node->next)
	    {
                Waypoint* wp = (Waypoint*) wp_node->item;
                Color w = Color_createFromRGBAf(1.0, 0.0, 0.0, 1.0);
                Physics_drawShape(body->shape, Point_add(wp->point, Point_createFromXY(320.0, 240.0)), w);
	    }

            Body_drawBody(body, time, color);
	}
    }

    MapIterator_delete(iter);
}


void Physics_update(BOOL do_update)
{
    MapIterator* iter = MapIterator_new();

    for(MapIterator_reset(body_map, iter); TRUE == MapIterator_valid(iter); MapIterator_step(iter))
    {
        Pair* pair = (Pair*) MapIterator_get(iter);
        LinkedList* list = (LinkedList*) pair->right;
        Node* node;

        for(node = list->first; NULL != node; node = node->next)
	{
            Body* body = (Body*) node->item;

            // Don't do anything with immovable bodies
            if(FALSE == body->immovable)
	    {
                // Update body with last waypoints data
                if(NULL != body->waypoints->last)
		{
                    Waypoint* wp = (Waypoint*) body->waypoints->last->item;

                    if(TRUE == do_update)
                    {
                        body->immovableNormal = Vector_create2d(0.0, 0.0);
                        body->immovableWeight = 1.0;
                        body->position = wp->point;
                        body->velocity = wp->velocity;
                        body->acceleration = Vector_create2d(0.0, 100.0);
                    }
		}
	    }

            // Clear waypoints
            Body_clearWaypoints(body);
	}
    }

    MapIterator_delete(iter);
}


static TIME _last_time = -10.0;
void Physics_checkCollisions(TIME time, BOOL do_update)
{
    MapIterator* iter = MapIterator_new();
    OrderedSet* collisions = OrderedSet_new();
    OrderedSetIterator* set_iter = OrderedSetIterator_new();
    LinkedList* recheck_list = LinkedList_new();

    if(_last_time < 0.0)
    {
        _last_time = time;
    }

    TIME delta_time = time - _last_time;
    _last_time = time;
    //delta_time *= 10.0;


    /////////////////////////////////
    // Reset waypoints of all objects
    /////////////////////////////////
    for(MapIterator_reset(body_map, iter); TRUE == MapIterator_valid(iter); MapIterator_step(iter))
    {
        Pair* pair = (Pair*) MapIterator_get(iter);
        LinkedList* list = (LinkedList*) pair->right;
        Node* node;

        for(node = list->first; NULL != node; node = node->next)
	{
            Body* body = (Body*) node->item;

            // Don't do anything with immovable bodies
            if(FALSE == body->immovable)
	    {
                // Update body with last waypoints data
                Point start = body->position;
                Point end = calculate_position(body->position, body->velocity, body->acceleration, delta_time);
                Vector velocity = calculate_velocity(body->velocity, body->acceleration, delta_time);

                Body_addWaypoint(body, Waypoint_newFromTPVA(0.0, start, body->velocity, body->acceleration));
                Body_addWaypoint(body, Waypoint_newFromTPVA(delta_time, end, velocity, body->acceleration));
	    }
            else
	    {
                Body_clearWaypoints(body);
                Body_addWaypoint(body, Waypoint_newFromTPVA(0.0, body->position, body->velocity, body->acceleration));
                Body_addWaypoint(body, Waypoint_newFromTPVA(delta_time, body->position, body->velocity, body->acceleration));
	    }
	}
    }


    //////////////////////////////////////////////////////////////
    // Calculate an initial list of collisions between all objects
    //////////////////////////////////////////////////////////////
    for(OrderedSetIterator_reset(collision_types, set_iter);
        OrderedSetIterator_valid(set_iter) != FALSE;
        OrderedSetIterator_step(set_iter))
    {
        _CollisionHookType* type = (_CollisionHookType*) OrderedSetIterator_get(set_iter);

        LinkedList* self_bodies = (LinkedList*) Map_get(body_map, type->self);
        LinkedList* other_bodies = (LinkedList*) Map_get(body_map, type->other);

        _detectCollisions(self_bodies, other_bodies, delta_time, collisions);
    }

    ///////////////////////////////////////////////////////////////
    // Keep poping collisions and handle them, until all collisions
    // are resolved
    ///////////////////////////////////////////////////////////////
    BOOL loop = TRUE;

    while(TRUE == loop)
    {
        if(recheck_list->length > 0)
        {
            LinkedList* self_bodies = LinkedList_new();
            Node* node;

            for(node = recheck_list->first; NULL != node; node = node->next)
            {
                Body* self_body = (Body*) node->item;
                LinkedList_addLast(self_bodies, self_body);

                for(OrderedSetIterator_reset(collision_types, set_iter);
                    OrderedSetIterator_valid(set_iter) != FALSE;
                    OrderedSetIterator_step(set_iter))
                {
                    _CollisionHookType* type = (_CollisionHookType*) OrderedSetIterator_get(set_iter);

                    if(type->self == self_body->type || type->other == self_body->type)
                    {
                        LinkedList* other_bodies;
                        if(type->self == self_body->type)
                        {
                           other_bodies  = (LinkedList*) Map_get(body_map, type->other);
                           _detectCollisions(self_bodies, other_bodies, delta_time, collisions);
                        }
                        else
                        {
                           other_bodies  = (LinkedList*) Map_get(body_map, type->self);
                           _detectCollisions(other_bodies, self_bodies, delta_time, collisions);
                        }
                    }
                }

                LinkedList_removeItem(recheck_list, self_body);
            }

            LinkedList_delete(self_bodies);
        }

        CollisionData* data = OrderedSet_popMin(collisions);
        if(NULL != data)
        {
            _CollisionHookType collision_hook_type;
            collision_hook_type.self = data->body[0]->type;
            collision_hook_type.other = data->body[1]->type;


            LinkedList* hooks = Map_get(collision_hooks[data->type], (void*) &collision_hook_type);

            if(NULL != hooks)
            {
                Node* node;
                for(node = hooks->first; NULL != node; node = node->next)
                {
                    Hook* hook = (Hook*) node->item;
                    int hook_result = Hook_call(hook, data);

                    if(hook_result & 1)
                    {
                        LinkedList_addLast(recheck_list, data->body[0]);
                        OrderedSet_removeIf(collisions, _isBodyInvolved, data);
                    }

                    if(hook_result & 2)
                    {
                        LinkedList_addLast(recheck_list, data->body[1]);
                        OrderedSet_removeIf(collisions, _isBodyInvolved, data);
                    }
                }
            }

            CollisionData_delete(data);
        }
        else
        {
            loop = FALSE;
        }
    }

    _Physics_drawBodies(body_map, 0.0, Color_createFromRGBAf(1.0, 1.0, 0.0, 1.0));

    Platform_refreshWindow();

    MapIterator_delete(iter);
    OrderedSet_delete(collisions);
    OrderedSetIterator_delete(set_iter);
    LinkedList_delete(recheck_list);
}

void Physics_setDefaultCollisionHandler(BODY_TYPE left, BODY_TYPE right)
{
    Hook* enter_hook = Hook_newFromFunction(NULL, _default_handleEnterCollision);
    Hook* in_hook = Hook_newFromFunction(NULL, _default_handleInCollision);
    Hook* exit_hook = Hook_newFromFunction(NULL, _default_handleExitCollision);


    Physics_setCollisionHandler(left, right, COLLISION_ENTER, enter_hook);
    Physics_setCollisionHandler(left, right, COLLISION_PERSIST, in_hook);
    //Physics_setCollisionHandler(left, right, COLLISION_EXIT, exit_hook);
}
