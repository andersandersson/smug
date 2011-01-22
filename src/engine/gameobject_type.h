#ifndef SMUG_ENGINE_GAMEOBJECT_TYPE_H
#define SMUG_ENGINE_GAMEOBJECT_TYPE_H

struct LinkedList;

typedef struct GameObject
{
    struct LinkedList* mSubObjects;
    struct GameObject* mParent;
    SmugType mTypes;
    BOOL (*hasAttribute)(struct GameObject* self, SmugAttribute attr);
    BOOL (*inheritAttribute)(struct GameObject* self, SmugAttribute attr, SmugInheritType type);
    void (*deleteMe)(void* data);
} GameObject;

#endif /* SMUG_ENGINE_GAMEOBJECT_TYPE_H */
