#ifndef SMUG_ENGINE_POSITIONEDOBJECT_H
#define SMUG_ENGINE_POSITIONEDOBJECT_H

#include <common/common.h>
#include <engine/positionedobject_type.h>

struct Vector;
struct Point;

PositionedObject* PositionedObject_new(void);

BOOL PositionedObject_setPos(PositionedObject* self, float x, float y);
BOOL PositionedObject_getPos(PositionedObject* self, struct Vector* v);
BOOL PositionedObject_getX(PositionedObject* self, float* x);
BOOL PositionedObject_getY(PositionedObject* self, float* y);
BOOL PositionedObject_moveTo(PositionedObject* self, float x, float y);
BOOL PositionedObject_getPosForDrawing(GameObject* self, struct Point* p);

/**
 * Called by platform when position will not change again during this heartbeat.
 */
BOOL PositionedObject_commitPosition(PositionedObject* self);

int PositionedObject_addObjectAt(PositionedObject* self, PositionedObject* other, float x, float y);

void PositionedObject_init(PositionedObject* self);
void PositionedObject_deInit(PositionedObject* self);

#endif // SMUG_ENGINE_POSITIONEDOBJECT_H
