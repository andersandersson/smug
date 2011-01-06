/** @file gameobject.h
  * @ingroup smug_engine
  * @brief Defines the GameObject type.
  */

/** @addtogroup smug_engine
  * @{
  */

#ifndef SMUG_ENGINE_GAMEOBJECT_H
#define SMUG_ENGINE_GAMEOBJECT_H

#include "common/common.h"
// #include "graphics/drawable/drawable.h"
struct Drawable;
#include "utils/sstring.h"
#include "utils/vector_type.h"
#include <utils/point_type.h>
#include <engine/interpoint.h>

/** The GameObject struct.
  */
typedef struct GameObject
{
	LinkedList* drawables;
	LinkedList* bodies;

    BOOL visible;

    String* tag;
    Interpoint* position;
} GameObject;

/** Constructor for GameObject.
  *
  * @relatesalso GameObject
  * @return A pointer to the GameObject just created.
  */
GameObject* GameObject_new(void);

/** Destructor for GameObject.
  *
  * @relatesalso GameObject
  * @param self A pointer to the GameObject to be deleted.
  */
void GameObject_delete(void* self);

void GameObject_setPos(GameObject* self, float x, float y);

void GameObject_moveTo(GameObject* self, float x, float y);

Vector GameObject_getPos(GameObject* self);

float GameObject_getX(GameObject* self);

float GameObject_getY(GameObject* self);

Point GameObject_getPosForDrawing(GameObject* self);

/**
 * Called by platform when position will not change again during this heartbeat.
 */
void GameObject_commitPosition(GameObject* self);

/** Assign a Drawable to the GameObject.
  *
  * @relatesalso GameObject
  * @param self A pointer to the GameObject.
  * @param d A pointer to the Drawable.
  */
void GameObject_addDrawable(GameObject* self, struct Drawable* d);

// struct Drawable* GameObject_getDrawable(GameObject* self, int drawableIndex);

void GameObject_removeDrawable(GameObject* self, struct Drawable* drawable);

void GameObject_setOpacity(GameObject* self, float opacity);

// int GameObject_addBody(GameObject* self, Body* b);

/* Render the GameObject.
  *
  * If the GameObject is visible and has a Drawable, that drawable is rendered.
  *
  * @relatesalso GameObject
  * @param self A pointer to the GameObject to render.
  */
// void GameObject_render(GameObject* self);

#endif //SMUG_ENGINE_GAMEOBJECT_H

/**@}*/
