#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "utils/linkedlist.h"
#include "gameobject.h"

typedef struct World
{
	LinkedList* gameObjects;

} World;

World* World_new();

void World_delete(void* world);

void World_addObject(World* world, GameObject* obj);

LinkedList* World_getObjectsByTag(World* world, char* tag);

#endif //WORLD_H
