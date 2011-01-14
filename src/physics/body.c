#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <utils/point.h>
#include <utils/vector.h>

#include <physics/body.h>

Body* Body_new(void)
{
    Body* body = malloc(sizeof(Body));

    body->type = 0;
    body->shape = NULL;
    body->position = Point_createFromXY(0.0, 0.0);
    body->velocity = Vector_create2d(0.0, 0.0);
    body->acceleration = Vector_create2d(0.0, 0.0);
    body->elasticity = 0.0;
    body->friction = 1.0;
    body->immovable = FALSE;
    
    body->waypoints = LinkedList_new();

    body->_newVelocity = Vector_create2d(0.0, 0.0);

    return body;
}

void Body_delete(Body* body)
{
    Node* node;

    for(node = body->waypoints->first; node != NULL; node = node->next)
      {
	Waypoint_delete((Waypoint*) node->item);
      }

    free(body->waypoints);
    free(body);
}

void Body_setShape(Body* body, Shape* shape)
{
    body->shape = shape;
}

void Body_setPosition(Body* body, Point position)
{
    body->position = position;
}

void Body_setVelocity(Body* body, Vector velocity)
{
    body->velocity = velocity;
    body->_newVelocity = velocity;
}

void Body_move(Body* body, float x, float y)
{
}

void Body_moveTo(Body* body, float x, float y)
{
}


void Body_clearWaypoints(Body* body)
{
  Node* node = NULL;

  while(body->waypoints->first != NULL)
    {
      LinkedList_remove(body->waypoints, body->waypoints->first);
    }
}

void Body_addWaypoint(Body* body, Waypoint* waypoint)
{
  LinkedList_addLast(body->waypoints, waypoint);
}

void Body_removeWaypointsAfterTime(Body* body, float time)
{
  Node* node = body->waypoints->first;
  Node* next;

  while(node != NULL)
    {
      Waypoint* wp = (Waypoint*) node->item;

      if(wp->time < time)
	{
	  node = node->next;
	}
      else
	{
	  next = node->next;
	  LinkedList_remove(body->waypoints, node);
	  node = next;
	}
    }
}

void Body_dump(Body* body)
{
  Node* node;
  char buffer[65536];
  
  DEBUG("===================");
  DEBUG("BODY: %d", body);
  DEBUG("-------------------");
  DEBUG("type: %d", body->type);
  DEBUG("immovable: %d", body->immovable);
  DEBUG("position: (%f,%f)", body->position.v.d[0], body->position.v.d[1]);
  DEBUG("velocity: (%f,%f)", body->velocity.d[0], body->velocity.d[1]);
  DEBUG("friction: %f", body->friction);
  DEBUG("elasticity: %f", body->elasticity);
  DEBUG("mass: %f", body->mass);

  sprintf(buffer, "");

  for(node = body->waypoints->first; node != NULL; node = node->next)
    {
      Waypoint* wp = (Waypoint*) node->item;
      sprintf(buffer, "%s(%f: %f,%f)", buffer, wp->time, wp->point.v.d[0], wp->point.v.d[1]);
      
      if(NULL != node->next)
	{
	  sprintf(buffer, "%s -> ", buffer);
	}
    }

  DEBUG("Waypoints: %s", buffer);
  DEBUG("-------------------");
  DEBUG("_newVelocity: (%f,%f)", body->_newVelocity.d[0], body->_newVelocity.d[1]);
  DEBUG("*******************");
}

void Body_dumpWaypoints(Body* body)
{
  Node* node;
  char buffer[65536];

  sprintf(buffer, "");

  for(node = body->waypoints->first; node != NULL; node = node->next)
    {
      Waypoint* wp = (Waypoint*) node->item;
      sprintf(buffer, "%s(%f: [%f,%f], [%f,%f])", buffer, wp->time, wp->point.v.d[0], wp->point.v.d[1], wp->velocity.d[0], wp->velocity.d[1]);
      
      if(NULL != node->next)
	{
	  sprintf(buffer, "%s -> ", buffer);
	}
    }

  DEBUG("%s", buffer);
}
