#ifndef SMUG_ENGINE_POSITION_OBJECT_TYPE_H
#define SMUG_ENGINE_POSITION_OBJECT_TYPE_H

// #include <engine/gameobject2.h>

#include <engine/gameobject.h>
struct Interpoint;
// struct GameObject;

typedef struct PositionObjectData
{
    struct Interpoint* mPosition;
    // struct Interpoint* mRelativePos;
    SmugInheritType mPositionInheritance;

    // void* mData; // For subtypes.
    // GameObject* mParent;

} PositionObjectData;

#endif /* SMUG_ENGINE_POSITION_OBJECT_TYPE_H */
