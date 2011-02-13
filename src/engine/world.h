/**
 * @file world.h
 * @brief Defines the World type.
 * @if doc_files
 * @ingroup smug_engine
 * @endif
 */

/**
 * @addtogroup smug_engine
 * @{
 */

#ifndef SMUG_ENGINE_WORLD_H
#define SMUG_ENGINE_WORLD_H

#include "common/common.h"
#include "utils/linkedlist.h"
#include "gameobject.h"

/** The World struct.
  * @ingroup smug_engine
  */
typedef struct World
{
    LinkedList* gameObjects;

} World;

/** Constructor for World.
 *
 * @relates World
 * @return A pointer to the World struct just created.
 */
World* World_new(void);

/** Destructor for World.
 *
 * Deletes all GameObject objects in the world as well as the world itself.
 *
 * @relates World
 * @param world A pointer to the World struct to be deleted.
 */
void World_delete(void* world);

/** Adds the passed GameObject to the world.
 *
 * Objects added to the World are owned by the World. The World is responsible for deleting them.
 *
 * @relates World
 * @param world The world.
 * @param obj A pointer to the GameObject to be added to the world.
 */
void World_addObject(World* world, GameObject* obj);

/** Get all objects in the world with a certain tag.
 *
 * Returns a LinkedList with all GameObject objects in the world matching the given tag.
 *
 * @relates World
 * @param world The world.
 * @param tag The tag to match against.
 * @return A newly created LinkedList with any matching GameObject objects.
 */
LinkedList* World_getObjectsByTag(World* world, char* tag);

void World_render(World* world);

#endif //SMUG_ENGINE_WORLD_H

/**@}*/
