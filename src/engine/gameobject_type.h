#ifndef SMUG_ENGINE_GAMEOBJECT_TYPE_H
#define SMUG_ENGINE_GAMEOBJECT_TYPE_H

#include <engine/gameobject_decl.h>

struct LinkedList;

typedef struct _GameObject
{
    struct LinkedList* mSubObjects;
    GameObject* mParent;
    SmugType mTypes;
    BOOL (*hasAttribute)(GameObject* self, SmugAttribute attr);
    BOOL (*inheritAttribute)(GameObject* self, SmugAttribute attr, SmugInheritType type);
    void (*deleteMe)(void* data);
} _GameObject;

#endif /* SMUG_ENGINE_GAMEOBJECT_TYPE_H */
