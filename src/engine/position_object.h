#ifndef SMUG_ENGINE_POSITION_OBJECT_H
#define SMUG_ENGINE_POSITION_OBJECT_H

#include <common/common.h>

struct Vector;
struct Point;
struct GameObject;

struct PositionedObject* PositionedObject_new(void);

BOOL PositionedObject_setPos(struct PositionedObject* self, float x, float y);
BOOL PositionedObject_getPos(struct PositionedObject* self, struct Vector* v);
BOOL PositionedObject_getX(struct PositionedObject* self, float* x);
BOOL PositionedObject_getY(struct PositionedObject* self, float* y);
BOOL PositionedObject_moveTo(struct PositionedObject* self, float x, float y);
BOOL PositionedObject_getPosForDrawing(struct PositionedObject* self, struct Point* p);
/**
 * Called by platform when position will not change again during this heartbeat.
 */
BOOL PositionedObject_commitPosition(struct PositionedObject* self);

int PositionedObject_addObjectAt(struct PositionedObject* self, struct PositionedObject* other, float x, float y);

void PositionedObject_init(struct PositionedObject* self);
void PositionedObject_deInit(struct PositionedObject* self);

#endif // SMUG_ENGINE_POSITION_OBJECT_H
