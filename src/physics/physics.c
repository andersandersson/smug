#include <math.h>
#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <graphics/color.h>
#include <utils/linkedlist.h>
#include <utils/vector.h>
#include <utils/rectangle.h>
#include <utils/point.h>
#include <utils/orderedset.h>
#include <utils/map.h>
#include <physics/debug.h>
#include <physics/physics.h>
#include <physics/calc.h>
#include <platform/platform.h>

#define EPSILON 0.0001

// Position epsilon
#define P_EPSILON 0.01

// Velocity epsilon
#define V_EPSILON 1.0

// Time epsilon
#define T_EPSILON 0.0001

#define static

Body* current_body = NULL;

/** Type used for the map of collision hooks */
typedef struct _CollisionHookType
{
    BODY_TYPE self;
    BODY_TYPE other;
} _CollisionHookType;

/** Map of all added bodies in the physics. Maps BODY_TYPE => LinkedList<Body*> */ 
static Map* body_map;

/** Map of all added collision hooks. Maps _CollisionType => LinkedList<Hook*> */
static Map* collision_hooks;

/** Whether or not the physics is initialized. */
static BOOL isInitialized = FALSE;

/** Whether or not to draw debug frames. */
static BOOL debugMode = FALSE;

/* FUNCTION DECLARATION */
static _CollisionHookType* _CollisionHookType_new(void);
static void _CollisionHookType_delete(_CollisionHookType* type);
static int _compareCollisionHookType(void* self, void* left, void* right);
static int _compareCollisionData(void* self, void* left, void* right);

static BOOL _addCollision(Map* collisions, Body* self, CollisionData* collision_data);

static BOOL _handleCollisions(Map* collisions, Map* hooks);
static BOOL _updateCollisions(Map* collisions);
static BOOL _removeCollision(Map* collisions, Body* self, Body* other);
static BOOL _inCollision(Map* collisions, Body* self, Body* other);
static void _clearCollisions(Map* collisions);
static void _clearCollisionsAfterTime(Map* collisions, float time);

int _default_handleEnterCollision(void* lparam, void* rparam);
int _default_handleInCollision(void* lparam, void* rparam);
int _default_handleExitCollision(void* lparam, void* rparam);

  
//===========================================
// STATIC FUNCTIONS (PRIVATE)
//===========================================

/** Allocate memory for _CollisionType */
static _CollisionHookType* _CollisionHookType_new(void)
{
    return malloc(sizeof(_CollisionHookType));
}

/** Free memory for _CollisionType */
static void _CollisionHookType_delete(_CollisionHookType* type)
{
    free(type);
}

/** Compare two _CollisionTypes, used for Map */
static int _compareCollisionHookType(void* self, void* left, void* right)
{
    _CollisionHookType* _self = (_CollisionHookType*) left;
    _CollisionHookType* _other = (_CollisionHookType*) right;
 
    if(_self->self == _other->self && _self->other == _other->other)
      {
	return 0;
      }
    else if(_self->self == _other->self && _self->other < _other->other)
      {
	return -1;
      }
    else if(_self->self == _other->self && _self->other > _other->other)
      {
	return 1;
      }
    else if(_self->self < _other->self)
      {
	return -1;
      }
    else
      {
	return 1;
      }
}


/** Compare two _CollisionKeys, used for Map */
static int _compareCollisionData(void* self, void* left, void* right)
{
    CollisionData* _self = (CollisionData*) left;
    CollisionData* _other = (CollisionData*) right;

    // Always let the same object equal itself
    if(_self == _other)
      {
	return 0;
      }

    // If the pair is already in the collision list, consider them to be the same
    if((_self->self == _other->self && _self->other == _other->other) ||
       (_self->self == _other->other && _self->other == _other->self))
      {
	return 0;
      }       

    if(_self->absoluteTime < _other->absoluteTime)
      {
	return -1;
      }

    if(_self->absoluteTime > _other->absoluteTime)
      {
	return 1;
      }

    if(_self->absoluteTime == _other->absoluteTime && _self->type == _other->type && _self->self == _other->self && _self->other == _other->other)
      {
	return 0;
      }
    else if(_self->absoluteTime == _other->absoluteTime && _self->type == _other->type && _self->self == _other->self && _self->other < _other->other)
      {
	return -1;
      }
    else if(_self->absoluteTime == _other->absoluteTime && _self->type == _other->type && _self->self == _other->self && _self->other > _other->other)
      {
	return 1;
      }
    else if(_self->absoluteTime == _other->absoluteTime && _self->type == _other->type && _self->self < _other->self)
      {
	return -1;
      }
    else if(_self->absoluteTime == _other->absoluteTime && _self->type == _other->type && _self->self > _other->self)
      {
	return 1;
      }
    else if(_self->absoluteTime == _other->absoluteTime && _self->type < _other->type)
      {
	return -1;
      }
    else if(_self->absoluteTime == _other->absoluteTime && _self->type > _other->type)
      {
	return 1;
      }
	// What should this return as default value?
	return 0;
}


/** Find collision for two moving rectangles
 *
 * This will find the collision time for two moving rectangles. It 
 * is done by comparing both rectangles projections in the X and Y
 * plane respectively, and see if these projections collide at the
 * same time.
 *
 * Since it is rectangles, the projections will simply be the interval
 * from its left/top x-/y-point to its right/bottom x-/y-point.
 */
static BOOL _collideRectangleRectangle(Body* self, Point* self_start, Vector* self_velocity, Vector* self_acceleration, 
				       Body* other, Point* other_start, Vector* other_velocity, Vector* other_acceleration,
				       float delta_time, LinkedList* collisions)
{
    CollisionData* collision_data;
  
    float self_x_start = Rectangle_getX(self->shape->data) + Point_getX(*self_start);
    float self_x_vel = Vector_getX(self_velocity);
    float self_x_acc = Vector_getX(self_acceleration);
    // float self_x_end = self_x_start + self_x_vel*delta_time + self_x_acc*0.5*delta_time*delta_time;
    float self_width = Rectangle_getW(self->shape->data);

    float other_x_start = Rectangle_getX(other->shape->data) + Point_getX(*other_start);
    float other_x_vel = Vector_getX(other_velocity);
    float other_x_acc = Vector_getX(other_acceleration);
    // float other_x_end = other_x_start + other_x_vel*delta_time + other_x_acc*0.5*delta_time*delta_time;
    float other_width = Rectangle_getW(other->shape->data);

    float self_y_start = Rectangle_getY(self->shape->data) + Point_getY(*self_start);
    float self_y_vel = Vector_getY(self_velocity);
    float self_y_acc = Vector_getY(self_acceleration);
    // float self_y_end = self_y_start + self_y_vel*delta_time + self_y_acc*0.5*delta_time*delta_time;
    float self_height = Rectangle_getH(self->shape->data);

    float other_y_start = Rectangle_getY(other->shape->data) + Point_getY(*other_start);
    float other_y_vel = Vector_getY(other_velocity);
    float other_y_acc = Vector_getY(other_acceleration);
    // float other_y_end = other_y_start + other_y_vel*delta_time + other_y_acc*0.5*delta_time*delta_time;
    float other_height = Rectangle_getH(other->shape->data);

    float t_x_in[2], t_x_out[2];
    int x_p = 0;

    float t_y_in[2], t_y_out[2];
    int y_p = 0;

    // Check for collision of the moving X-projections of the rectangle
    x_p = _collideMovingInterval1D(self_x_start, self_x_vel, self_x_acc, self_width,
				   other_x_start, other_x_vel, other_x_acc, other_width,
				   &t_x_in[0], &t_x_out[0], &t_x_in[1], &t_x_out[1]);

    y_p = _collideMovingInterval1D(self_y_start, self_y_vel, self_y_acc, self_height,
				   other_y_start, other_y_vel, other_y_acc, other_height,
				   &t_y_in[0], &t_y_out[0], &t_y_in[1], &t_y_out[1]);

    float wayout_x;
    float wayout_y;	  
    
    BOOL overlap_x = FALSE;
    BOOL overlap_y = FALSE;
    BOOL overlap_start = FALSE;

    BOOL same_x_speed = FALSE;
    BOOL same_y_speed = FALSE;

    int side;
    // float overlap;
    
    side = _collideInterval1D(self_x_start, self_width, other_x_start, other_width, &wayout_x);
    //DEBUG("WO-X: (%f,%f) vs (%f,%f) - %f", self_x_start, self_height, other_x_start, other_height, wayout_y);
    if(wayout_x > P_EPSILON)
      {
	overlap_x = TRUE;
	same_x_speed = TRUE;
	
	if(side == -1 || (side == 0 && self < other))
	  {
	    wayout_x *= -1.0;
	  }	  
      }

    side = _collideInterval1D(self_y_start, self_height, other_y_start, other_height, &wayout_y);
    //DEBUG("WO-Y: (%f,%f) vs (%f,%f) - %f", self_y_start, self_height, other_y_start, other_height, wayout_y);
    if(wayout_y > P_EPSILON)
      {
	overlap_y = TRUE;
	same_y_speed = TRUE;

	if(side == -1 || (side == 0 && self < other))
	  {
	    wayout_y *= -1.0;
	  }	  
      }
    
    if(0 == x_p && TRUE == overlap_x)
      {
	x_p = 1;
	t_x_in[0] = 0.0;
	t_x_out[0] = delta_time;
      }

    if(0 == y_p && TRUE == overlap_y)
      {
	y_p = 1;
	t_y_in[0] = 0.0;
	t_y_out[0] = delta_time;
      }

    overlap_start = overlap_x * overlap_y;

    if(TRUE == overlap_start)
      {
	collision_data = CollisionData_new();
	collision_data->self = self;
	collision_data->other = other;

	if(fabs(wayout_x) > fabs(wayout_y))
	  {
	    collision_data->wayout = Vector_create2d( 0.0, wayout_y );
	  }
	else
	  {
	    collision_data->wayout = Vector_create2d( wayout_x, 0.0 );
	  }

	collision_data->time = 0.0;
	collision_data->type = COLLISION_PERSIST;
	LinkedList_addLast(collisions, collision_data);
      }

    int i;
    int j;

    for(i=0; i<x_p; i++)
      for(j=0; j<y_p; j++)
	{
	  float t_in = 0.0;
	  float t_out = delta_time;
	  float t_mid = 0.0;

	  t_in = max(t_x_in[i], t_y_in[j]);

	  t_out = min(t_x_out[i], t_y_out[j]);	  

	  t_mid = (t_in + t_out)*0.5;

	  if(FALSE == overlap_start && t_in < delta_time && t_mid > 0.0)
	    {	
	      Point end_point = Point_addVector(*self_start, Vector_add(Vector_multiply(*self_velocity, t_in),
									Vector_multiply(*self_acceleration, t_in*t_in*0.5)));

	      Point dest_point = Point_addVector(*self_start, Vector_add(Vector_multiply(*self_velocity, t_in),
									 Vector_multiply(*self_acceleration, delta_time*delta_time*0.5)));
	      
	      float distance_left = Vector_length(Point_distanceToPoint(end_point, dest_point));

	      if(distance_left > P_EPSILON)
		{
		  collision_data = CollisionData_new();
		  collision_data->self = self;
		  collision_data->other = other;
		  collision_data->type = COLLISION_ENTER;
		  collision_data->time =  t_in;
		  
		  // If X-axis enters before Y-axis
		  if(t_x_in[i] > t_y_in[j] && FALSE == same_x_speed) {
		    collision_data->normal = Vector_create2d( (self_x_start - other_x_start) / fabs(self_x_start - other_x_start), 0.0f);
		  } else {
		    collision_data->normal = Vector_create2d( 0.0f, (self_y_start - other_y_start) / fabs(self_y_start - other_y_start));
		  }

		  LinkedList_addLast(collisions, collision_data);
		}
	    }

	  if(t_out < delta_time)
	    {
	      collision_data = CollisionData_new();
	      collision_data->self = self;
	      collision_data->other = other;
	      collision_data->type = COLLISION_EXIT;
	      collision_data->time =  t_out;
		
	      if(t_x_out[i] < t_y_out[j]) {
		collision_data->normal = Vector_create2d( wayout_x / fabs(wayout_x), 0.0f);
	      } else {
		collision_data->normal = Vector_create2d( 0.0f, wayout_y / fabs(wayout_y));
	      }

	      LinkedList_addLast(collisions, collision_data);
	    }
	}
	// TODO: What should this function return?
	return TRUE;
}


static BOOL _detectCollisions(LinkedList* self, LinkedList* other, float delta_time, OrderedSet* collisions)
{
    LinkedList* new_collisions = LinkedList_new();

    Node* self_node;
    Node* other_node;

    // Point self_start;
    // Point self_end;
    // Point other_start;
    // Point other_end;

    BOOL changes_made = FALSE;

    if(NULL == self->first || NULL == other->first)
        {
            return FALSE;
        }

    for(self_node = self->first; self_node != NULL; self_node = self_node->next)
        {
            for(other_node = other->first; other_node != NULL; other_node = other_node->next)
                {		  
                    if(other_node->item == self_node->item)
                        {
			  continue;
                        }

                    Body* self_body = (Body*) self_node->item;
                    Body* other_body = (Body*) other_node->item;

                    if(NULL == self_body->shape || NULL == other_body->shape)
                        {
			  continue;
                        }
		    
		    Node* self_left_wp_node = self_body->waypoints->first;
		    Node* self_right_wp_node = self_body->waypoints->first->next;

		    Node* other_left_wp_node = other_body->waypoints->first;
		    Node* other_right_wp_node = other_body->waypoints->first->next;

		    while(self_right_wp_node != NULL && other_right_wp_node != NULL)
		      {
			Waypoint* self_left_wp = (Waypoint*) self_left_wp_node->item;
			Waypoint* self_right_wp = (Waypoint*) self_right_wp_node->item;
			Waypoint* other_left_wp = (Waypoint*) other_left_wp_node->item;
			Waypoint* other_right_wp = (Waypoint*) other_right_wp_node->item;

			float t_start = max(self_left_wp->time, other_left_wp->time);
			float t_end = min(self_right_wp->time, other_right_wp->time);

			if(t_start != t_end)
			  {
			    Vector self_velocity = self_left_wp->velocity;
			    Vector other_velocity = other_left_wp->velocity;

			    float self_delta_time = t_start - self_left_wp->time;
			    float other_delta_time = t_start - other_left_wp->time;

			    Point self_start = Point_addVector(self_left_wp->point, Vector_add(Vector_multiply(self_left_wp->velocity, self_delta_time),
											       Vector_multiply(self_body->acceleration, self_delta_time*self_delta_time*0.5)));
			    Point other_start = Point_addVector(other_left_wp->point, Vector_add(Vector_multiply(other_left_wp->velocity, other_delta_time),
											       Vector_multiply(other_body->acceleration, other_delta_time*other_delta_time*0.5)));

			    if(SHAPE_RECTANGLE == self_body->shape->type &&
			       SHAPE_RECTANGLE == other_body->shape->type)
			      {
				//DEBUG("%f -> %f", t_start, t_end);
				if(TRUE == _collideRectangleRectangle(self_body, &self_start, &self_velocity, &self_body->acceleration, 
								      other_body, &other_start, &other_velocity, &other_body->acceleration,
								      t_end - t_start, new_collisions))
				  {
				    changes_made = TRUE;
				    
				    while(new_collisions->first != NULL)
				      {
					CollisionData* data = (CollisionData*) new_collisions->first->item;
					float dt = data->time;

					data->selfStart = self_start;
					data->otherStart = other_start;
					data->selfMovement = Vector_add(Vector_multiply(self_left_wp->velocity, dt), Vector_multiply(self_body->acceleration, dt*dt*0.5));
					data->otherMovement = Vector_add(Vector_multiply(other_left_wp->velocity, dt), Vector_multiply(other_body->acceleration, dt*dt*0.5));
					
					data->selfVelocity = self_left_wp->velocity;
					data->otherVelocity = other_left_wp->velocity;
					data->absoluteTime = t_start + data->time;
					data->absoluteTimeStart = t_start;
					data->deltaTime = delta_time;
					
					OrderedSet_insert(collisions, data);
					
					LinkedList_removeItem(new_collisions, (void*) data);
				      } // while
				  } // if (TRUE == _collide)
			      } // if (SHAPE_RECTANGLE)
			  } // if (t_start != t_end)

			if(self_right_wp->time < other_right_wp->time)
			  {
			    self_left_wp_node = self_right_wp_node;
			    self_right_wp_node = self_right_wp_node->next;
			  }
			else
			  {
			    other_left_wp_node = other_right_wp_node;
			    other_right_wp_node = other_right_wp_node->next;
			  }			
		      } // while
		} // for
        } // for
    
    LinkedList_delete(new_collisions);

    return changes_made;
}


int _default_handleEnterCollision(void* lparam, void* rparam)
{
  CollisionData* data = (CollisionData*) rparam;

  DEBUG("ENTER [0x%x] (w 0x%x) COLLISION: %f", data->self, data->other, data->absoluteTime);

  Vector movement = data->selfMovement;

  float m1 = data->self->mass;
  float m2 = data->other->mass;

  //Vector_print(data->selfVelocity);
  Vector self_velocity = Vector_add(data->selfVelocity, Vector_multiply(data->self->acceleration, data->absoluteTime - data->absoluteTimeStart));
  //DEBUG("Times: %f, %f, %f, %f", data->absoluteTime, data->absoluteTimeStart, data->deltaTime, data->absoluteTime - data->absoluteTimeStart);
  //Vector_print(Vector_multiply(data->self->acceleration, data->absoluteTime - data->absoluteTimeStart));
  Vector other_velocity = Vector_add(data->otherVelocity, Vector_multiply(data->other->acceleration, data->absoluteTime - data->absoluteTimeStart));

  Vector velocity_self_n = Vector_projection(self_velocity, data->normal);
  Vector velocity_self_p = Vector_sub(self_velocity, velocity_self_n);

  // Vector velocity_other_n = Vector_projection(other_velocity, data->normal);

  float v1 = Vector_dotProduct(self_velocity, data->normal);
  float v2 = Vector_dotProduct(other_velocity, data->normal);
  float new_v1;

  if(data->other->immovable == TRUE)
    {
      new_v1 = -1.0 * v1 + 2 * v2;
    } 
  else
    {
      new_v1 = (m1 - m2) / (m1 + m2) * v1 + (2 * m2) / (m1 + m2) * v2;
    }

  Vector new_velocity = Vector_add(velocity_self_p, Vector_multiply(data->normal, new_v1*data->self->elasticity));
  Vector next_velocity = Vector_add(new_velocity, Vector_multiply(data->self->acceleration, data->deltaTime - data->absoluteTime));
  Point new_position = Point_addVector(data->selfStart, movement);
  Point next_position = Point_addVector(new_position, Vector_add(
								 Vector_multiply(new_velocity, data->deltaTime - data->absoluteTime), 
								 Vector_multiply(data->self->acceleration, (data->deltaTime - data->absoluteTime)*(data->deltaTime - data->absoluteTime)*0.5)));

  float new_length = Vector_length(new_velocity);

  if(fabs(new_v1 - v1) < V_EPSILON)
    {
      //DEBUG("IGNORED CHANGE");
      //return FALSE;
    }
  //DEBUG("NEW LENGTH:  %f", new_length);

  //Body_dumpWaypoints(data->self);
  Vector_print(new_velocity);
  Body_removeWaypointsAfterTime(data->self, data->absoluteTime);
  Body_addWaypoint(data->self, Waypoint_createFromTimePositionVelocity(data->absoluteTime, new_position, new_velocity));

  if(new_length > V_EPSILON)
    {
      Body_addWaypoint(data->self, Waypoint_createFromTimePositionVelocity(data->deltaTime, next_position, next_velocity));
      data->self->_newVelocity = next_velocity;
    }
  else
    {
      DEBUG("RESET V");
      data->self->_newVelocity = Vector_create2d(0.0, 0.0);
    }

  Body_dumpWaypoints(data->self);
  return TRUE;
}

int _default_handleInCollision(void* lparam, void* rparam)
{
  CollisionData* data = (CollisionData*) rparam;
  // float length = Vector_length(data->wayout);
  Node* node;

  if(data->self->immovable == TRUE)
    {
      return FALSE;
    }
  
  Vector wayout = data->wayout;

  if(data->other->immovable == FALSE)
    {
      wayout = Vector_multiply(data->wayout, 0.5);
    }

  BOOL result = FALSE;

  DEBUG("[0x%x]: (%f, %f)", data->self, wayout.d[0], wayout.d[1]);

  for(node = data->self->waypoints->first; node != NULL; node = node->next)
    {
      Waypoint* wp = (Waypoint*)node->item;
      Point p = wp->point;
      
      wp->point = Point_addVector(wp->point, wayout);

      if(Point_getX(p) != Point_getX(wp->point) || Point_getY(p) != Point_getY(wp->point))
	{
	  result = TRUE;
	}
    }

  return result;
}

int _default_handleExitCollision(void* lparam, void* rparam)
{
  // CollisionData* data = (CollisionData*) rparam;
  //DEBUG("COLLISION EXIT: %f", data->time);
  return FALSE;
}

//===========================================
// PUBLIC FUNCTIONS
//===========================================

CollisionData* CollisionData_new(void)
{
    return malloc(sizeof(CollisionData));
}


void CollisionData_delete(CollisionData* data)
{
    free(data);
}


int Physics_init(void)
{
    smug_assert(!isInitialized);

    // float t;
    body_map = Map_new();

    collision_hooks = Map_new();
    Map_setCompare(collision_hooks, _compareCollisionHookType);

    DEBUG("Initializing physics");
    isInitialized = TRUE;
    return 1;
}


BOOL Physics_isInitialized(void)
{
    return isInitialized;
}


void Physics_terminate(void)
{
    smug_assert(isInitialized);

    /*
    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList_delete((LinkedList*) node->value);
        }
    */

    Map_delete(body_map);
    Map_delete(collision_hooks);
    
    isInitialized = FALSE;
}


void Physics_setCollisionHandler(BODY_TYPE self, BODY_TYPE other, COLLISION_TYPE type, Hook* hook)
{
    _CollisionHookType _type;

    _type.self = self;
    _type.other = other;


    // Add the collision hook to the map of hooks

    Map* map = Map_get(collision_hooks, (void*) &_type);

    if(NULL == map)
        {
            _CollisionHookType* _type = _CollisionHookType_new();
            _type->self = self;
            _type->other = other;

            map = Map_new();

            Map_set(collision_hooks, (void*) _type, map);
        }

    Map_set(map, (void*) type, hook);
}


void Physics_addBody(Body* body)
{
    LinkedList* list = Map_get(body_map, (void*) body->type);
    DEBUG("Adding body");

    if(NULL == list)
        {
            list = LinkedList_new();
            Map_set(body_map, (void*) body->type, list);
        }

    LinkedList_addLast(list, body);
}


void Physics_removeBody(Body* body)
{
    LinkedList* list = Map_get(body_map, (void*) body->type);

    if(NULL != list)
        {
            LinkedList_removeItem(list, body);
        }
}


float _last_time = -10.0;

void _Physics_drawBodies(float time, Color color)
{
  MapIterator* iter = MapIterator_new();

  for(MapIterator_reset(body_map, iter); TRUE == MapIterator_valid(iter); MapIterator_step(iter))
    {
      Pair* pair = (Pair*) MapIterator_get(iter);
      LinkedList* list = (LinkedList*) pair->right;
      Node* node;

      for(node = list->first; NULL != node; node = node->next)
	{
	  Body* body = (Body*) node->item;

	  Body_drawBody(body, time, color);
	  
	  Node* wp_node;

	  for(wp_node = body->waypoints->first; NULL != wp_node; wp_node = wp_node->next)
	    {
	      Waypoint* wp = (Waypoint*) wp_node->item;
	      Physics_drawShape(body->shape, wp->point, color);
	    }
	}
    }

  MapIterator_delete(iter);
}

void Physics_update(TIME time, BOOL do_update)
{
  MapIterator* iter = MapIterator_new();
  OrderedSet* collisions = OrderedSet_new();
  OrderedSetIterator* set_iter = OrderedSetIterator_new();
  
  if(_last_time < 0.0)
    {
      _last_time = time;
    }
  
  TIME delta_time = time - _last_time;
  _last_time = time;
  delta_time *= 10.0;
  
  
  /////////////////////////////////
  // Reset waypoints of all objects
  /////////////////////////////////
  for(MapIterator_reset(body_map, iter); TRUE == MapIterator_valid(iter); MapIterator_step(iter))
    {
      Pair* pair = (Pair*) MapIterator_get(iter);
      LinkedList* list = (LinkedList*) pair->right;
      Node* node;
      
      for(node = list->first; NULL != node; node = node->next)
	{
	  Body* body = (Body*) node->item;

	  // Don't do anything with immovable bodies
	  if(FALSE == body->immovable)
	    {
	      // Update body with last waypoints data
	      if(NULL != body->waypoints->last)
		{
		  Waypoint* wp = (Waypoint*) body->waypoints->last->item;
		  body->position = wp->point;
		  body->velocity = wp->velocity;
		}
	      
	      // Clear waypoints
	      Body_clearWaypoints(body);	  	  

	      // Reset acceleration to default
	      body->acceleration = Vector_create2d(0.0, 10.0);
	      
	      Point start = body->position;
	      Point end = calculate_position(body->position, body->velocity, body->acceleration, delta_time);
	      Vector velocity = calculate_velocity(body->velocity, body->acceleration, delta_time);
	      
	      Body_addWaypoint(body, Waypoint_createFromTimePositionVelocity(0.0, start, body->velocity));
	      Body_addWaypoint(body, Waypoint_createFromTimePositionVelocity(0.0, end, velocity));	  
	    }
	  else
	    {
	      Body_clearWaypoints(body);	  	  
	      Body_addWaypoint(body, Waypoint_createFromTimePositionVelocity(0.0, body->position, body->velocity));
	    }
	}
      }
 
  
  //////////////////////////////////////////////////////////////
  // Calculate an initial list of collisions between all objects
  //////////////////////////////////////////////////////////////
  for(MapIterator_reset(collision_hooks, iter); TRUE == MapIterator_valid(iter); MapIterator_step(iter))
    {
      Pair* pair = (Pair*) MapIterator_get(iter);
      _CollisionHookType* type = (_CollisionHookType*) pair->left;

      LinkedList* self_bodies = (LinkedList*) Map_get(body_map, type->self);
      LinkedList* other_bodies = (LinkedList*) Map_get(body_map, type->other);

      _detectCollisions(self_bodies, other_bodies, delta_time, collisions);
    }
  
  for(OrderedSetIterator_reset(collisions, set_iter); TRUE == OrderedSetIterator_valid(set_iter); OrderedSetIterator_step(set_iter))
    {
    }
  _Physics_drawBodies(0.0, Color_createFromRGBAf(1.0, 0.0, 0.0, 1.0));

  Platform_refreshWindow();

  MapIterator_delete(iter);
  OrderedSet_delete(collisions);
  OrderedSetIterator_delete(set_iter);
  /*
            LinkedList* list;
            Node* list_node;

            list = (LinkedList*) node->value;

            for(list_node = list->first; list_node != NULL; list_node = list_node->next)
                {
                    Body* body = list_node->item;
		    Node* n = body->waypoints->last;
		    Waypoint* wp;

		    if(NULL != n && TRUE == do_update)
		      {
			Body_setPosition(body, ((Waypoint*) n->item)->point);
			Body_setVelocity(body, ((Waypoint*) n->item)->velocity);			
		      }
		    
		    if(TRUE == do_update || NULL == n)
		      {
			Body_clearWaypoints(body);

			if(FALSE == body->immovable)
			  {			 
			    body->acceleration = Vector_add(body->acceleration, Vector_create2d(0.0, 30.0));
			    Body_addWaypoint(body, Waypoint_createFromTimePositionVelocity(0.0, body->position, body->velocity));

			    Vector velocity_end = Vector_add(body->velocity, Vector_multiply(body->acceleration, delta_time*delta_time*0.5));			    
			    Point next_position = Point_addVector(body->position, Vector_add(Vector_multiply(body->velocity, delta_time), Vector_multiply(body->acceleration, delta_time*delta_time*0.5)));

			    Body_addWaypoint(body, Waypoint_createFromTimePositionVelocity(delta_time, next_position, velocity_end));
			  }
			else
			  {
			    Body_addWaypoint(body, Waypoint_createFromTimePositionVelocity(0.0, body->position, body->velocity));
			    Body_addWaypoint(body, Waypoint_createFromTimePositionVelocity(delta_time, body->position, body->velocity));
			  }
		      }
		}
        }
    

    _clearCollisions(collision_list);

    // Iterate over all objects with an associated CollisionHook and detect
    // their collision. Without hooks their will be no handling of the collision
    // and thus no need to detect them.
    for(node = collision_hooks->first; node != NULL; node = node->next)
      {
	LinkedList* self_list;
	LinkedList* other_list;
	
	self_list = Map_get(body_map, ((_CollisionHookType *) node->key)->self);
	other_list = Map_get(body_map, ((_CollisionHookType *) node->key)->other);
	
	if(NULL != self_list && NULL != other_list)
	  {
	    _detectCollisions(self_list, other_list, delta_time, collision_list);
	  }
      }
    
    // Go through and handle collisions until no ones are left
    while(collision_list->first != NULL)
      {
	MapNode* node = collision_list->first;
	CollisionData* data = (CollisionData*) node->key;
	LinkedList_remove(collision_list, data);
	DEBUG("NODE COLLIUDE 0x%x", data);
      }
    
    //_handleCollisions(collision_list, collision_hooks);
    //Platform_refreshWindow();

    // Update all bodies with their handled new_positions.
    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList* list;
            Node* list_node;

            list = (LinkedList*) node->value;

            for(list_node = list->first; list_node != NULL; list_node = list_node->next)
                {
                    Body* body = list_node->item;
		    
		    if(FALSE == body->immovable)
		      {			
			body->acceleration = Vector_create2d(0.0, 0.0);
			//body->velocity = body->_newVelocity;
		      }
                }
        }
    */
}

void Physics_setDefaultCollisionHandler(BODY_TYPE left, BODY_TYPE right)
{
  Hook* enter_hook = Hook_newFromFunction(NULL, _default_handleEnterCollision);
  Hook* in_hook = Hook_newFromFunction(NULL, _default_handleInCollision);
  Hook* exit_hook = Hook_newFromFunction(NULL, _default_handleExitCollision);


  Physics_setCollisionHandler(left, right, COLLISION_ENTER, enter_hook);
  Physics_setCollisionHandler(left, right, COLLISION_PERSIST, in_hook);
  Physics_setCollisionHandler(left, right, COLLISION_EXIT, exit_hook);
}


