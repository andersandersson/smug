/** @file gameobject.h
  * @brief Defines the GameObject type.
  */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "common.h"

/** The GameObject struct.
  */
typedef struct GameObject
{
	float x;
	float y;
	
	void* shape;
	void* sprite;
	
	char* tag;

} GameObject;

/** Constructor for GameObject.
  *
  * @relatesalso GameObject
  * @return A pointer to the GameObject just created.
  */
GameObject* GameObject_new();

/** Destructor for GameObject.
  *
  * @relatesalso GameObject
  * @param obj A pointer to the GameObject to be deleted.
  */
void GameObject_delete(void* obj);

#endif //GAMEOBJECT_H
