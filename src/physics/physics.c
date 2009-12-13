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
    BODY_TYPE left;
    BODY_TYPE right;
} _CollisionType;

static Map* body_map;
static Map* collision_hooks;
static LinkedList* collision_list;
static BOOL isInitialized = FALSE;

static _CollisionType* _CollisionType_new()
{
    return malloc(sizeof(_CollisionType));
}

static void _CollisionType_delete(_CollisionType* type)
{
    free(type);
}

static BOOL _compareCollisionType(void* left, void* right)
{
    if(((_CollisionType*) left)->left == ((_CollisionType*) right)->left &&
       ((_CollisionType*) left)->right == ((_CollisionType*) right)->right)
        {
            return TRUE;
        }

    return FALSE;
}

CollisionData* CollisionData_new()
{
    return malloc(sizeof(CollisionData));
}

void CollisionData_delete(CollisionData* data)
{
    free(data);
}

int Physics_init()
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

BOOL Physics_isInitialized()
{
    return isInitialized;
}

void Physics_terminate()
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

void Physics_addCollisionHook(BODY_TYPE left, BODY_TYPE right, Hook* hook)
{
    _CollisionType type;

    type.left = left;
    type.right = right;

    LinkedList* list = Map_get(collision_hooks, (void*) &type);

    if(NULL == list)
        {
            _CollisionType* _type = _CollisionType_new();
            _type->left = left;
            _type->right = right;
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

BOOL Physics_collidePoints1D(float x1_start, float x1_end, float x2_start, float x2_end, float *t)
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

BOOL Physics_collideInterval1D(float i1_x1_start, float i1_x1_end, float i1_x2_start, float i1_x2_end,
                               float i2_x1_start, float i2_x1_end, float i2_x2_start, float i2_x2_end,
                               float* t_in, float* t_out)
{
    float x1_in = 0;
    float x1_out = 0;
    float x2_in = 0;
    float x2_out = 0;

    BOOL res = TRUE;

    res *= (1 - Physics_collidePoints1D(i1_x1_start, i1_x1_end, i2_x1_start, i2_x1_end, &x1_in));
    res *= (1 - Physics_collidePoints1D(i1_x1_start, i1_x1_end, i2_x2_start, i2_x2_end, &x1_out));
    res *= (1 - Physics_collidePoints1D(i1_x2_start, i1_x2_end, i2_x1_start, i2_x1_end, &x2_in));
    res *= (1 - Physics_collidePoints1D(i1_x2_start, i1_x2_end, i2_x2_start, i2_x2_end, &x2_out));

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

BOOL Physics_collideRectangleRectangle(Body* left, Body* right, CollisionData** collision_data)
{
    float t_x_in = 0;
    float t_x_out = 0;
    float t_y_in = 0;
    float t_y_out = 0;

    float left_x_start = Rectangle_getX(left->shape->data) + Point_getX(&left->position);
    float left_x_end = Rectangle_getX(left->shape->data) + Point_getX(&left->new_position);
    float left_width = Rectangle_getW(left->shape->data);

    float right_x_start = Rectangle_getX(right->shape->data) + Point_getX(&right->position);
    float right_x_end = Rectangle_getX(right->shape->data) + Point_getX(&right->new_position);
    float right_width = Rectangle_getW(right->shape->data);

    float left_y_start = Rectangle_getY(left->shape->data) + Point_getY(&left->position);
    float left_y_end = Rectangle_getY(left->shape->data) + Point_getY(&left->new_position);
    float left_height = Rectangle_getH(left->shape->data);

    float right_y_start = Rectangle_getY(right->shape->data) + Point_getY(&right->position);
    float right_y_end = Rectangle_getY(right->shape->data) + Point_getY(&right->new_position);
    float right_height = Rectangle_getH(right->shape->data);

    BOOL res = TRUE;

    res *= (1 - Physics_collideInterval1D(left_x_start, left_x_end, left_x_start+left_width, left_x_end+left_width,
                                          right_x_start, right_x_end, right_x_start+right_width, right_x_end+right_width,
                                          &t_x_in, &t_x_out));

    res *= (1 - Physics_collideInterval1D(left_y_start, left_y_end, left_y_start+left_height, left_y_end+left_height,
                                          right_y_start, right_y_end, right_y_start+right_height, right_y_end+right_height,
                                          &t_y_in, &t_y_out));

    if(TRUE == res)
        {
            return FALSE;
        }

    // If either overlap exists before the other one enters
    if(t_x_out < t_y_in || t_y_out < t_x_in)
        {
            return FALSE;
        }


    if(t_x_in < 1.0 && t_y_in < 1.0 && t_x_out > 0.0 && t_y_out > 0.0) {
        (*collision_data) = CollisionData_new();
        (*collision_data)->left = left;
        (*collision_data)->right = right;
        (*collision_data)->collisionTime = t_x_in > t_y_in ? t_x_in : t_y_in;
        (*collision_data)->movement = Point_distanceToPoint(left->position, left->new_position);

        return TRUE;
    }

    return FALSE;
}

void Physics_detectCollisions(LinkedList* left, LinkedList* right, LinkedList* collisions)
{
    CollisionData* collision_data;
    Vector result;
    Node* left_node;
    Node* right_node;

    if(NULL == left->first || NULL == right->first)
        {
            return;
        }

    for(left_node = left->first; left_node != NULL; left_node = left_node->next)
        {
            for(right_node = right->first; right_node != NULL; right_node = right_node->next)
                {
                    if(right_node->item == left_node->item)
                        {
                            continue;
                        }

                    Body* left_body = (Body*) left_node->item;
                    Body* right_body = (Body*) right_node->item;

                    if(NULL == left_body->shape || NULL == right_body->shape)
                        {
                            continue;
                        }

                    if(SHAPE_RECTANGLE == left_body->shape->type &&
                       SHAPE_RECTANGLE == right_body->shape->type)
                        {
                            if(TRUE == Physics_collideRectangleRectangle(left_body, right_body, &collision_data))
                                {
                                    LinkedList_addLast(collisions, collision_data);
                                }
                        }
                }
        }
}

void Physics_handleCollisions(LinkedList* collisions, LinkedList* hooks)
{
    Node* node;

    for(node = collisions->first; node != NULL; node = node->next)
        {
            CollisionData* data = (CollisionData*) node->item;

            Hook_callAll(hooks, data);
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
            LinkedList* left;
            LinkedList* right;

            left = Map_get(body_map, ((_CollisionType *) node->key)->left);
            right = Map_get(body_map, ((_CollisionType *) node->key)->right);

            if(NULL != left && NULL != right)
                {
                    Physics_detectCollisions(left, right, collision_list);
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
            Physics_handleCollisions(collision_list, (LinkedList*) node->value);
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
