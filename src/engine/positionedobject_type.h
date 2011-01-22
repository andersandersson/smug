#ifndef SMUG_ENGINE_POSITION_OBJECT_TYPE_H
#define SMUG_ENGINE_POSITION_OBJECT_TYPE_H

#include <engine/gameobject.h>
struct Interpoint;

typedef struct PositionedObject
{
    GameObject base; // Inherit from GameObject
    struct Interpoint* mPosition;
    SmugInheritType mPositionInheritance;
} PositionedObject;

#endif /* SMUG_ENGINE_POSITION_OBJECT_TYPE_H */
