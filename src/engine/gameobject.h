/** @file gameobject.h
  * @brief Defines the GameObject type.
  */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "common/common.h"
#include "graphics/drawable.h"

/** The GameObject struct.
  */
typedef struct GameObject
{
    float x;
    float y;
    
    void* shape;
    Drawable* drawable;
    BOOL visible;
    
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

/** Assign a Drawable to the GameObject.
  *
  * @relatesalso GameObject
  * @param obj A pointer to the GameObject.
  * @param d A pointer to the Drawable.
  */
void GameObject_setDrawable(GameObject* obj, Drawable* d);

/** Render the GameObject.
  *
  * If the GameObject is visible and has a Drawable, that drawable is rendered.
  * 
  * @relatesalso GameObject
  * @param obj A pointer to the GameObject to render.
  */
void GameObject_render(GameObject* obj);

#endif //GAMEOBJECT_H
