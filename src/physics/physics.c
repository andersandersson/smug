#include "physics.h"
#include "debug.h"

#include "common/log.h"

#include "utils/linkedlist.h"
#include "utils/vector.h"

typedef struct _CollisionHookList {
    BODY_TYPE left;
    BODY_TYPE right;
    LinkedList* hooks;
} _CollisionHookList;


static LinkedList* body_list;
static LinkedList* collision_hooks;

int Physics_init()
{
    body_list = LinkedList_new();
    collision_hooks = LinkedList_new();

    DEBUG("Initializing physics");

    return 1;
}

void Physics_terminate()
{   
    LinkedList_delete(body_list);
    LinkedList_delete(collision_hooks);
}

void Physics_addCollisionHook(BODY_TYPE left, BODY_TYPE right, Hook* hook)
{
    Node* node;
    _CollisionHookList* hooklist;

    for(node = collision_hooks->first; node != collision_hooks->last; node = node->next)
        {
            hooklist = (_CollisionHookList*) node->item;
        }
}

void Physics_addBody(Body* body)
{
    DEBUG("Adding body");
    LinkedList_addLast(body_list, body);
}

void Physics_removeBody(Body* body)
{
    LinkedList_removeItem(body_list, body);
}

void Physics_update(TIME time)
{
    Node* node;
    Body* body;

    for(node = body_list->first; node != NULL; node = node->next)
        {
            body = (Body*) node->item;
            
            body->new_position = Point_addVector(body->new_position, body->movement);
            
            Physics_drawShape(body->shape, body->position);


            body->position = body->new_position;
        }
}

