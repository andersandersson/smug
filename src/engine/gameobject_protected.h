#ifndef SMUG_ENGINE_GAMEOBJECT_PROTECTED_H
#define SMUG_ENGINE_GAMEOBJECT_PROTECTED_H

#include <engine/gameobject.h>

void GameObject_deInit(GameObject* self);
void GameObject_init(GameObject* self);

InternalGameObject* InternalGameObject_getParent(InternalGameObject* self);
InternalGameObject* InternalGameObject_getChild(InternalGameObject* self);
GameObject* InternalGameObject_getAsGeneric(InternalGameObject* self);
InternalGameObject* InternalGameObject_getMostSpecific(InternalGameObject* self);
BOOL InternalGameObject_isExactType(InternalGameObject* self, SmugType type);
InternalGameObject* InternalGameObject_inherit(InternalGameObject* self, void* data);
void* InternalGameObject_getData(InternalGameObject* self, SmugType type);
char* InternalGameObject_getTypeString(SmugType type);

#endif /* SMUG_ENGINE_GAMEOBJECT_PROTECTED_H */
