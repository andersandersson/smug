#ifndef SMUG_ENGINE_POSITIONEDOBJECT_INTERNAL_H
#define SMUG_ENGINE_POSITIONEDOBJECT_INTERNAL_H

#include <engine/gameobject_internal.h>

struct Interpoint;

typedef struct _PositionedObject
{
    _GameObject base; // Inherit from GameObject
    struct Interpoint* mPosition;
    SmugInheritType mPositionInheritance;
} _PositionedObject;

#endif /* SMUG_ENGINE_POSITIONEDOBJECT_INTERNAL_H */
