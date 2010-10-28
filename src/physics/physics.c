#include "physics.h"
#include "debug.h"

#include "common/log.h"

#include "utils/linkedlist.h"
#include "utils/vector.h"
#include "utils/map.h"

#include <math.h>
#include <stdlib.h>

typedef struct _CollisionType
{
    BODY_TYPE self;
    BODY_TYPE other;
} _CollisionType;

static Map* body_map;
static Map* collision_hooks;
static LinkedList* collision_list;
static BOOL isInitialized = FALSE;
static BOOL debugMode = FALSE;

static _CollisionType* _CollisionType_new(void);
static void _CollisionType_delete(_CollisionType* type);
static BOOL _compareCollisionType(void* self, void* other);
static BOOL _collidePoints1D(float x1_start, float x1_end, float x2_start, float x2_end, float *t);
static BOOL _collideInterval1D(float i1_x1_start, float i1_x1_end, float i1_x2_start, float i1_x2_end, 
			       float i2_x1_start, float i2_x1_end, float i2_x2_start, float i2_x2_end, 
			       float* t_in, float* t_out);
static BOOL _collideRectangleRectangle(Body* self, Body* other, CollisionData** collision_data);
static void _detectCollisions(LinkedList* self, LinkedList* other, LinkedList* collisions);
static void _handleCollisions(LinkedList* collisions, LinkedList* hooks);
  
static _CollisionType* _CollisionType_new(void)
{
    return malloc(sizeof(_CollisionType));
}

static void _CollisionType_delete(_CollisionType* type)
{
    free(type);
}

static BOOL _compareCollisionType(void* self, void* other)
{
    if(((_CollisionType*) self)->self == ((_CollisionType*) other)->self &&
       ((_CollisionType*) self)->other == ((_CollisionType*) other)->other)
        {
            return TRUE;
        }

    return FALSE;
}

static BOOL _collideMovingPoints1D(float x1_start, float x1_end, float x2_start, float x2_end, float *t)
{
    float denumerator, numerator;

    numerator = x1_start - x2_start;
    denumerator = x2_end - x1_end + x1_start - x2_start;

    if(denumerator == 0) {
        return FALSE;
    }

    *t = numerator / denumerator;

    return TRUE;
}

static BOOL _collideMovingInterval1D(float i1_x1_start, float i1_x1_end, float i1_x2_start, float i1_x2_end,
                               float i2_x1_start, float i2_x1_end, float i2_x2_start, float i2_x2_end,
                               float* t_in, float* t_out)
{
    float x1_in = 0;
    float x1_out = 0;
    float x2_in = 0;
    float x2_out = 0;

    BOOL res = TRUE;

    res *= (1 - _collideMovingPoints1D(i1_x1_start, i1_x1_end, i2_x1_start, i2_x1_end, &x1_in));
    res *= (1 - _collideMovingPoints1D(i1_x1_start, i1_x1_end, i2_x2_start, i2_x2_end, &x1_out));
    res *= (1 - _collideMovingPoints1D(i1_x2_start, i1_x2_end, i2_x1_start, i2_x1_end, &x2_in));
    res *= (1 - _collideMovingPoints1D(i1_x2_start, i1_x2_end, i2_x2_start, i2_x2_end, &x2_out));

    if(TRUE == res)
        {
            if(max(i1_x1_start, i1_x2_start) <= min(i2_x1_start, i2_x2_start) ||
               max(i2_x1_start, i2_x2_start) <= min(i1_x1_start, i1_x2_start))
                {
                    return FALSE;
                }
            else
                {
                    *t_in = -100000.0;
                    *t_out = 100000.0;
                    return TRUE;
                }
        }

    if(x1_in > x1_out) swap_float(&x1_in, &x1_out);
    if(x2_in > x2_out) swap_float(&x2_in, &x2_out);

    *t_in = x1_in < x2_in ? x1_in : x2_in;
    *t_out = x1_out > x2_out ? x1_out : x2_out;

    return TRUE;
}

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

    res *= (1 - _collideMovingInterval1D(self_x_start, self_x_end, self_x_start+self_width, self_x_end+self_width,
					 other_x_start, other_x_end, other_x_start+other_width, other_x_end+other_width,
					 &t_x_in, &t_x_out));

    res *= (1 - _collideMovingInterval1D(self_y_start, self_y_end, self_y_start+self_height, self_y_end+self_height,
					 other_y_start, other_y_end, other_y_start+other_height, other_y_end+other_height,
					 &t_y_in, &t_y_out));

    if(TRUE == res)
        {
            return FALSE;
        }

    if(t_x_out < t_y_in || t_y_out < t_x_in)
        {
            return FALSE;
        }

    if(t_x_in < 1.0 && t_y_in < 1.0 && t_x_out > 0.0 && t_y_out > 0.0) {
        (*collision_data) = CollisionData_new();
        (*collision_data)->self = self;
        (*collision_data)->other = other;
	
	if(t_x_in > t_y_in) {
	  (*collision_data)->collisionTime =  t_x_in;
	  (*collision_data)->contactNormal = Vector_create2d( (self_x_start - other_x_start) / abs(self_x_start - other_x_start), 0.0f);
	} else {
	  (*collision_data)->collisionTime =  t_y_in;
	  (*collision_data)->contactNormal = Vector_create2d(0.0f, (self_y_start - other_y_start) / abs(self_y_start - other_y_start));
	}
        
	(*collision_data)->selfMovement = Point_distanceToPoint(self->position, self->new_position);

        return TRUE;
    }

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

static void _handleCollisions(LinkedList* collisions, LinkedList* hooks)
{
    Node* node;

    for(node = collisions->first; node != NULL; node = node->next)
        {
            CollisionData* data = (CollisionData*) node->item;

            Hook_callAll(hooks, data);
        }
}

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

    for(list_node = collision_list->last; list_node != NULL; list_node = list_node->prev)
        {
            CollisionData_delete(list_node->item);
            LinkedList_remove(collision_list, list_node);
        }

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

    for(node = collision_hooks->first; node != NULL; node = node->next)
        {
            _handleCollisions(collision_list, (LinkedList*) node->value);
        }

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
