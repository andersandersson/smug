#include "waypoint.h"

#include "common/log.h"

#include <stdlib.h>

Waypoint* Waypoint_new(void)
{
    Waypoint* waypoint = malloc(sizeof(Waypoint));

    waypoint->time = 0.0;    

    return waypoint;
}

Waypoint* Waypoint_createFromTimePositionVelocity(float time, Point p, Vector v)
{
  Waypoint* wp = Waypoint_new();

  wp->point = p;
  wp->time = time;
  wp->velocity = v;

  return wp;
}

void Waypoint_delete(Waypoint* waypoint)
{
    free(waypoint);
}

int Waypoint_compare(void* left, void* right)
{
  Waypoint* _left = (Waypoint*) left;
  Waypoint* _right = (Waypoint*) right;

  if(_left->time < _right->time)
    {
      return -1;
    }

  if(_left->time > _right->time)
    {
      return 1;
    }

  return 0;
}

