/**
 * @file gameobject.h
 * @brief Defines the GameObject functions.
 * @if doc_files
 * @ingroup smug_engine
 * @endif
 */

/**
 * @addtogroup smug_engine
 * @{
 */

#ifndef SMUG_ENGINE_GAMEOBJECT_H
#define SMUG_ENGINE_GAMEOBJECT_H

#include <common/common.h>

#include <engine/gameobject_type.h>

struct LinkedListIterator;

#define MAX_INHERITANCE_DEPTH 4


typedef unsigned int SmugType;

#define SMUG_TYPE_NONE               (0x0)
#define SMUG_TYPE_OBJECT             (0x0001)
#define SMUG_TYPE_POSITIONED         (0x0002)
#define SMUG_TYPE_BODY               (0x0004)
#define SMUG_TYPE_DRAWABLE           (0x0008)
#define SMUG_TYPE_SHAPE              (0x0010)
#define SMUG_TYPE_IMAGE              (0x0020)
#define SMUG_TYPE_ANIMATED_SPRITE    (0x0040)
#define SMUG_TYPE_TIMER              (0x0080)
#define SMUG_TYPE_DATA               (0x0100)
#define SMUG_TYPE_ANY                (0xFFFFFFFF)

typedef enum SmugAttribute
{
    SMUG_ATTR_COLOR,           // SMUG_COLOR_INHERIT, SMUG_COLOR_KEEP, SMUG_COLOR_BLEND
    SMUG_ATTR_OPACITY,         // SMUG_OPACITY_INHERIT, SMUG_OPACITY_KEEP
    SMUG_ATTR_VISIBILITY,      // SMUG_VISIBILITY_INHERIT, SMUG_VISIBILITY_KEEP
    SMUG_ATTR_POSITION,        // SMUG_POSITION_INHERIT, SMUG_POSITION_KEEP, SMUG_POSITION_RELATIVE
    SMUG_ATTR_WEIGHT           // SMUG_WEIGHT_COLLECT, SMUG_WEIGHT_LOCAL
} SmugAttribute;

typedef enum SmugInheritType
{
    SMUG_COLOR_INHERIT,
    SMUG_COLOR_KEEP,
    SMUG_COLOR_BLEND,
    SMUG_OPACITY_INHERIT,
    SMUG_OPACITY_KEEP,
    SMUG_VISIBILITY_INHERIT,
    SMUG_VISIBILITY_KEEP,
    SMUG_POSITION_INHERIT,
    SMUG_POSITION_KEEP,
    SMUG_POSITION_RELATIVE,
    SMUG_WEIGHT_COLLECT,
    SMUG_WEIGHT_LOCAL,
    SMUG_INHERIT_UNDEFINED
} SmugInheritType;

int GameObject_addObject(GameObject* self, GameObject* obj);
void GameObject_removeObject(GameObject* self, GameObject* child);
BOOL GameObject_hasChildObjects(GameObject* self);
int GameObject_nrChildObjects(GameObject* self);
BOOL GameObject_isRootObject(GameObject* self);
struct LinkedListIterator* GameObject_getChildIterator(GameObject* self);
GameObject* GameObject_getParent(GameObject* self);

BOOL GameObject_isType(GameObject* self, SmugType type);
BOOL GameObject_isExactType(GameObject* self, SmugType type);
BOOL GameObject_hasAttribute(GameObject* self, SmugAttribute attr);
BOOL GameObject_inheritAttribute(GameObject* self, SmugAttribute attr, SmugInheritType type);

void GameObject_doRecursive(GameObject* self, void(*function)(GameObject*));

/** Constructor for a generic GameObject. A generic GameObject can only be used to attach other objects to.
 *
 * @relates GameObject
 * @return A pointer to the GameObject just created.
 */
GameObject* GameObject_newGeneric(void);

/** Destructor for any GameObject.
 *
 * @relates GameObject
 * @param self A pointer to the GameObject to be deleted.
 */
void GameObject_delete(void* self);

#endif //SMUG_ENGINE_GAMEOBJECT_H

/**@}*/
