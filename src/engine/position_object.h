#ifndef SMUG_ENGINE_POSITION_OBJECT_H
#define SMUG_ENGINE_POSITION_OBJECT_H

#include <common/common.h>

struct Vector;
struct Point;
struct GameObject;

struct GameObject* PositionObject_new();

BOOL PositionObject_setPos(struct GameObject* self, float x, float y);
BOOL PositionObject_getPos(struct GameObject* self, struct Vector* v);
BOOL PositionObject_getX(struct GameObject* self, float* x);
BOOL PositionObject_getY(struct GameObject* self, float* y);
BOOL PositionObject_moveTo(struct GameObject* self, float x, float y);
BOOL PositionObject_getPosForDrawing(struct GameObject* self, struct Point* p);
/**
 * Called by platform when position will not change again during this heartbeat.
 */
BOOL PositionObject_commitPosition(struct GameObject* self);

int PositionObject_addObjectAt(struct GameObject* self, struct GameObject* other, float x, float y);

#endif // SMUG_ENGINE_POSITION_OBJECT_H
