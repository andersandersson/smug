/**
 * @file waypoint.h
 * @brief
 * @if doc_files
 * @ingroup smug_physics
 * @endif
 */

/**
 * @addtogroup smug_physics
 * @{
 */

#ifndef SMUG_PHYSICS_WAYPOINT_H
#define SMUG_PHYSICS_WAYPOINT_H

#include "utils/point.h"
#include "utils/vector.h"

typedef struct Waypoint
{
    float time;
    Point point;
    Vector velocity;
    Vector acceleration;
} Waypoint;

Waypoint* Waypoint_new(void);
Waypoint* Waypoint_newFromTPVA(float time, Point p, Vector v, Vector a);

void Waypoint_delete(Waypoint* waypoint);
int Waypoint_compare(void* left, void* right);

#endif // SMUG_PHYSICS_WAYPOINT_H

/**@}*/
