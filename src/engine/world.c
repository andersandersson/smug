#include <stdlib.h>
#include "world.h"
#include "string.h"

static void World_invariant(World* w)
{
    assert(NULL != w);
    assert(NULL != w->gameObjects);
}

World* World_new(void)
{
    World* world = malloc(sizeof(World));
    world->gameObjects = LinkedList_new();
    World_invariant(world);
    return world;
}

void World_delete(void* world)
{
    World* w = (World*)world;
    World_invariant(w);

    LinkedList_deleteContents(w->gameObjects, GameObject_delete);
    LinkedList_delete(w->gameObjects);
    free(w);
}

void World_addObject(World* world, GameObject* obj)
{
    World_invariant(world);
    LinkedList_addLast(world->gameObjects, obj);
//    DEBUG("%i objects in World.", LinkedList_length(world->gameObjects));
}

LinkedList* World_getObjectsByTag(World* world, char* tag)
{
    World_invariant(world);
    Node* node = world->gameObjects->first;
    LinkedList* newList = LinkedList_new();
    while (NULL != node)
    {
        if (0 == strcmp(((GameObject*)node->item)->tag, tag))
        {
            LinkedList_addLast(newList, (GameObject*)node->item);
        }
        node = node->next;
    }
    return newList;
}

void World_render(World* world)
{
    World_invariant(world);
//    LinkedList_doList(world->gameObjects, GameObject_render);
    Node* node;
    node = world->gameObjects->first;
    while (NULL != node)
    {
        //GameObject_render((GameObject*)node->item);
        node = node->next;
    }
}
