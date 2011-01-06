#ifndef SMUG_ENGINE_INTERPOINT_H
#define SMUG_ENGINE_INTERPOINT_H

#include <utils/point_type.h>
#include <common/common.h>

typedef struct
{
    Point point;
    Point oldPoint;
    BOOL pointCommitted;
    BOOL pointChangedThisHeartbeat;
} Interpoint;

Interpoint* Interpoint_new(Point p);

void Interpoint_delete(Interpoint* self);

Point Interpoint_getPoint(Interpoint* self);

void Interpoint_setTo(Interpoint* self, Point p);

void Interpoint_moveTo(Interpoint* self, Point p);

void Interpoint_commit(Interpoint* self);

Point Interpoint_getInterpolated(Interpoint* self);

#endif /* SMUG_ENGINE_INTERPOINT_H */
