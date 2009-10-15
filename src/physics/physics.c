#include "physics.h"
#include "debug.h"

#include "common/log.h"

#include "utils/linkedlist.h"
#include "utils/vector.h"
#include "utils/map.h"

#include <math.h>

typedef struct _CollisionType
{
    BODY_TYPE left;
    BODY_TYPE right;
} _CollisionType;

static Map* body_map;
static Map* collision_hooks;

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

int Physics_init()
{
    body_map = Map_new();
    collision_hooks = Map_new();
    collision_hooks->compareKeys = _compareCollisionType;

    DEBUG("Initializing physics");

    return 1;
}

void Physics_terminate()
{   
    MapNode* node;

    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList_delete((LinkedList*) node->value);
        }

    Map_delete(body_map);
    Map_delete(collision_hooks);
}

BOOL Physics_intervalOverlap(float left_l, float left_r, float right_l, float right_r, float* dest)
{
    float min_l = (left_l < left_r ? left_l : left_r);
    float max_l = (left_l > left_r ? left_l : left_r);
    float min_r = (right_l < right_r ? right_l : right_r);
    float max_r = (right_l > right_r ? right_l : right_r);

    if(min_l < min_r)
        {
            if(max_l > min_r)
                {
                    *dest = min_r - max_l;
                    return TRUE;
                }
        }
    else
        {
            if(max_r > min_l)
                {
                    *dest = max_r - min_l;
                    return TRUE;
                }
        }

    return FALSE;
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

BOOL Physics_collideRectangleRectangle(Body* left, Body* right, Vector* result)
{
    float left_x = Rectangle_getX((Rectangle*)left->shape->data) + Point_getX(&(left->position));
    float left_y = Rectangle_getY((Rectangle*)left->shape->data) + Point_getY(&(left->position));
    float left_w = Rectangle_getW((Rectangle*)left->shape->data);
    float left_h = Rectangle_getH((Rectangle*)left->shape->data);
    
    float right_x = Rectangle_getX((Rectangle*)right->shape->data) + Point_getX(&(right->position));
    float right_y = Rectangle_getY((Rectangle*)right->shape->data) + Point_getY(&(right->position));
    float right_w = Rectangle_getW((Rectangle*)right->shape->data);
    float right_h = Rectangle_getH((Rectangle*)right->shape->data);
    
    float overlap_x, overlap_y;
    
    if(TRUE == Physics_intervalOverlap(left_x, left_x+left_w, right_x, right_x+right_w, &overlap_x) &&
       TRUE == Physics_intervalOverlap(left_y, left_y+left_h, right_y, right_y+right_h, &overlap_y))
        {
            if(fabs(overlap_x) > fabs(overlap_y))
                {
                    Vector_setX(result, 0.0);
                    Vector_setY(result, overlap_y);
                }
            else
                {
                    Vector_setX(result, overlap_x);
                    Vector_setY(result, 0.0);
                }
            return TRUE;
        }

    return FALSE;
}

void Physics_detectCollisions(LinkedList* left, LinkedList* right, LinkedList* collisionHooks)
{
    CollisionData collision_data;
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
                    
                    if(NULL != left_body->shape && NULL != right_body->shape)
                        {
                            continue;
                        }
                    
                    if(SHAPE_RECTANGLE == left_body->shape->type &&
                       SHAPE_RECTANGLE == right_body->shape->type)
                        {
                            if(TRUE == Physics_collideRectangleRectangle(left_body, right_body, &result))
                                {
                                    collision_data.left = left_body;
                                    collision_data.right = right_body;
                                    collision_data.result = &result;
                                    Hook_callAll(collisionHooks, (void*) &collision_data);
                                }
                        }
                }
        }
}

void Physics_update(TIME time)
{
    MapNode* node;

    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList* list;
            Node* list_node;
            
            list = (LinkedList*) node->value;

            for(list_node = list->first; list_node != NULL; list_node = list_node->next)
                {
                    Body* body = list_node->item;

                    body->new_position = Point_addVector(body->new_position, body->movement);
                    
                    body->position = body->new_position;
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
                    Physics_detectCollisions(left, right, (LinkedList*) node->value);
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

                    Physics_drawShape(body->shape, body->position);
                }
        }
}
