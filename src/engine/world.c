#include <stdlib.h>
#include "world.h"
#include "string.h"

World* World_new()
{
	World* world = malloc(sizeof(World));
	world->gameObjects = LinkedList_new();
}

void World_delete(void* world)
{
	if (NULL != world)
	{
		World* w = (World*)world;
		if (NULL != w->gameObjects)
		{
			LinkedList_deleteContents(w->gameObjects, GameObject_delete);
			LinkedList_delete(w->gameObjects);
		}
		free(w);
	}
}

void World_addObject(World* world, GameObject* obj)
{
	LinkedList_addLast(world->gameObjects, obj);
}

LinkedList* World_getObjectsByTag(World* world, char* tag)
{
	Node* node = world->gameObjects->first;
	LinkedList* newList = LinkedList_new();
	while (NULL != node)
	{
		if (0 == strcmp(((GameObject*)node->item)->tag, tag))
		{
			LinkedList_add(newList, (GameObject*)node->item);
		}
		node = node->next;
	}
	return newList;
}
