/** @file world.h
  * @brief Defines the World type.
  */

#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "utils/linkedlist.h"
#include "gameobject.h"

/** The World struct.
  */
typedef struct World
{
	LinkedList* gameObjects;

} World;

/** Constructor for World.
  *
  * @relatesalso World
  * @return A pointer to the World struct just created.
  */
World* World_new();

/** Destructor for World.
  * 
  * Deletes all GameObject objects in the world as well as the world itself.
  * 
  * @relatesalso World
  * @param world A pointer to the World struct to be deleted.
  */
void World_delete(void* world);

/** Adds the passed GameObject to the world.
  * 
  * Objects added to the World are owned by the World. The World is responsible for deleting them.
  * 
  * @relatesalso World
  * @param world The world.
  * @param obj A pointer to the GameObject to be added to the world.
  */
void World_addObject(World* world, GameObject* obj);

/** Get all objects in the world with a certain tag.
  *
  * Returns a LinkedList with all GameObject objects in the world matching the given tag.
  * 
  * @relatesalso World
  * @param world The world.
  * @param tag The tag to match against.
  * @return A newly created LinkedList with any matching GameObject objects.
  */
LinkedList* World_getObjectsByTag(World* world, char* tag);

void World_render(World* world);

#endif //WORLD_H
