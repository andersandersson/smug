#include "physics.h"
#include "debug.h"

#include "common/log.h"

#include "utils/linkedlist.h"
#include "utils/vector.h"
#include "utils/map.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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

/** Map of collision types. Maps (type => LinkedList<type>) */
static Map* collision_types;

/** Map with CollisionData for each collision that occured the last tick. Maps Body* => Map<CollisionData* => NULL> */
static Map* collision_list;

/** Whether or not the physics is initialized. */
static BOOL isInitialized = FALSE;

/** Whether or not to draw debug frames. */
static BOOL debugMode = FALSE;

/* FUNCTION DECLARATION */
static _CollisionHookType* _CollisionHookType_new(void);
static void _CollisionHookType_delete(_CollisionHookType* type);
static int _compareCollisionHookType(void* self, void* other);
static int _compareCollisionData(void* self, void* other);

static BOOL _addCollision(Map* collisions, Body* self, CollisionData* collision_data);

static BOOL _handleCollisions(Map* collisions, Map* hooks);
static BOOL _updateCollisions(Map* collisions);
static BOOL _removeCollision(Map* collisions, Body* self, Body* other);
static BOOL _inCollision(Map* collisions, Body* self, Body* other);
static void _clearCollisions(Map* collisions);
static void _clearCollisionsAfterTime(Map* collisions, float time);

BOOL _default_handleEnterCollision(void* lparam, void* rparam);
BOOL _default_handleInCollision(void* lparam, void* rparam);
BOOL _default_handleExitCollision(void* lparam, void* rparam);

  
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
static int _compareCollisionHookType(void* self, void* other)
{
    _CollisionHookType* _self = (_CollisionHookType*) self;
    _CollisionHookType* _other = (_CollisionHookType*) other;
 
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
static int _compareCollisionData(void* self, void* other)
{
    CollisionData* _self = (CollisionData*) self;
    CollisionData* _other = (CollisionData*) other;

    // Always let the same object equal itself
    if(_self == _other)
      {
	return 0;
      }

    // If the pair is already in the collision list, consider them to be the same
    if(_self->self == _other->self && _self->other == _other->other ||
       _self->self == _other->other && _self->other == _other->self)
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
}


int _collideInterval1D(float x1_start, float x1_width, float x2_start, float x2_width, float *width)
{
  float i1 = x1_start + x1_width - x2_start;
  float i2 = x2_start + x2_width - x1_start;

  if(fabs(i1) < fabs(i2))
    {
      *width = i1;
      return -1;
    }
  else if(fabs(i1) > fabs(i2))
    {
      *width = i2;
      return 1;
    }
  else 
    {
      *width = i1;
      return 0;
    }
}

/** Find the collision time of two moving points in 1 dimension.
 *
 * Given two points on the X-axis, we will find when (if) they collide.
 *
 * @param x1_start First points starting position
 * @param x1_end   First points ending position
 * @param x2_start Second points starting position
 * @param x2_end   Second points ending position
 * @param[out] t   Time for collision
 *
 * @return Whether or not the points collide
 */
 int _collideMovingPoints1D(float x1, float v1, float a1, float x2, float v2, float a2, float *t1, float *t2)
{
    BOOL result = TRUE;

    if(a1 != a2)
      {
	float q = 2.0 * (x1 - x2) / (a1 - a2);
	float p = 2.0 * (v1 - v2) / (a1 - a2);
	
	if(p*p < 4.0*q)
	  {
	    *t1 = FP_NAN;
	    *t2 = FP_NAN;
	    result = 0;
	  }
	else if(p*p == 4.0*q)
	  {
	    *t1 = -p/2.0;
	    *t2 = FP_NAN;
	    result = 1;
	  }
	else
	  {
	    float sq = sqrt((p/2.0)*(p/2.0) - q);
	    *t1 = -p/2.0 - sq;
	    *t2 = -p/2.0 + sq;
	    result = 2;
	  }
      }
    else
      {
	if(v1 == v2)
	  {
	    if(x1 == x2)
	      {
		*t1 = 0.0;
		*t2 = 0.0;
		result = 1;
	      }
	    else
	      {
		*t1 = FP_NAN;
		*t2 = FP_NAN;
		result = 0;
	      }
	  }
	else
	  {
	    *t1 = -(x1 - x2) / (v1 - v2);
	    *t2 = FP_NAN;
	    result = 1;
	  }
      }

    return result;
}


/** Find the collision time for two moving intervals in 1 dimension.
 *
 * Given two intervals on the X-axis, we will find when (if) they collide. 
 * This is done by comparing the collision times of the moving ending points
 * for each interval.
 *
 * The moving intervals can be interpreted as:
 *
 * 0s: [i1_x1_start]----[i1_x2_start]
 * 0s:                                    [i2_x1_start]-----------[i2_x2_start]
 *
 * 1s:         [i1_x1_end]----[i1_x2_end]
 * 1s:                         [i2_x1_end]-----------[i2_x2_end]
 *
 * @relatesalso _collideMovingPoints1D
 *
 * @param i1_x1_start  First intervals left starting point
 * @param i1_x1_end    First intervals left ending point
 * @param i1_x2_start  First intervals right starting point
 * @param i1_x2_end    First intervals right ending point
 * @param i2_x1_start  Second intervals left starting point
 * @param i2_x1_end    Second intervals left ending point
 * @param i2_x2_start  Second intervals right starting point
 * @param i2_x2_end    Second intervals right ending point
 * @param[out] t_in    Starting time for interval overlap
 * @param[out] t_out   Ending time for interval overlap
 *
 * @return Whether or not the intervals are moving with the same speed
 */
 int _collideMovingInterval1D(float i1_start, float i1_vel, float i1_acc, float i1_width,
				     float i2_start, float i2_vel, float i2_acc, float i2_width,
				    float* t1_in, float* t1_out, float* t2_in, float* t2_out)
{
    float t_left_1, t_left_2, t_right_1, t_right_2;
    int num1; 
    int num2;
    
    // Check the first intervals right point against the second intervals left point
    num1 = _collideMovingPoints1D(i1_start+i1_width, i1_vel, i1_acc, i2_start, i2_vel, i2_acc, &t_left_1, &t_left_2);
    num2 = _collideMovingPoints1D(i1_start, i1_vel, i1_acc, i2_start+i2_width, i2_vel, i2_acc, &t_right_1, &t_right_2);

    if(1 == num1 && 1 == num2)
      {
	*t1_in = min(t_left_1, t_right_1);
	*t1_out = max(t_left_1, t_right_1);

	*t2_in = FP_NAN;
	*t2_out = FP_NAN;

	return 1;
      }
    else if(2 == num1 && 2 == num2)
      {
	*t1_in = min(min(t_left_1, t_left_2), min(t_right_1, t_right_2));
	*t1_out = max(min(t_left_1, t_left_2), min(t_right_1, t_right_2));

	*t2_in = min(max(t_left_1, t_left_2), max(t_right_1, t_right_2));
	*t2_out = max(max(t_left_1, t_left_2), max(t_right_1, t_right_2));

	return 2;
      }
    else if(2 == num1 && 0 == num2)
      {
	*t1_in = min(t_left_1, t_left_2);
	*t1_out = max(t_left_1, t_left_2);

	*t2_in = FP_NAN;
	*t2_out = FP_NAN;

	return 1;
      }
    else if(0 == num1 && 2 == num2)
      {
	*t1_in = min(t_right_1, t_right_2);
	*t1_out = max(t_right_1, t_right_2);

	*t2_in = FP_NAN;
	*t2_out = FP_NAN;

	return 1;
      }

    *t1_in = FP_NAN;
    *t1_out = FP_NAN;
    
    *t2_in = FP_NAN;
    *t2_out = FP_NAN;

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
  
    float self_x_start = Rectangle_getX(self->shape->data) + Point_getX(self_start);
    float self_x_vel = Vector_getX(self_velocity);
    float self_x_acc = Vector_getX(self_acceleration);
    float self_x_end = self_x_start + self_x_vel*delta_time + self_x_acc*0.5*delta_time*delta_time;
    float self_width = Rectangle_getW(self->shape->data);

    float other_x_start = Rectangle_getX(other->shape->data) + Point_getX(other_start);
    float other_x_vel = Vector_getX(other_velocity);
    float other_x_acc = Vector_getX(other_acceleration);
    float other_x_end = other_x_start + other_x_vel*delta_time + other_x_acc*0.5*delta_time*delta_time;
    float other_width = Rectangle_getW(other->shape->data);

    float self_y_start = Rectangle_getY(self->shape->data) + Point_getY(self_start);
    float self_y_vel = Vector_getY(self_velocity);
    float self_y_acc = Vector_getY(self_acceleration);
    float self_y_end = self_y_start + self_y_vel*delta_time + self_y_acc*0.5*delta_time*delta_time;
    float self_height = Rectangle_getH(self->shape->data);

    float other_y_start = Rectangle_getY(other->shape->data) + Point_getY(other_start);
    float other_y_vel = Vector_getY(other_velocity);
    float other_y_acc = Vector_getY(other_acceleration);
    float other_y_end = other_y_start + other_y_vel*delta_time + other_y_acc*0.5*delta_time*delta_time;
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
    float overlap;
    
    side = _collideInterval1D(self_x_start, self_width, other_x_start, other_width, &wayout_x);
    //DEBUG("WO-X: (%f,%f) vs (%f,%f) - %f", self_x_start, self_height, other_x_start, other_height, wayout_y);
    if(wayout_x > P_EPSILON)
      {
	overlap_x = TRUE;
	same_x_speed = TRUE;
	
	if(side == -1 || side == 0 && self < other)
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

	if(side == -1 || side == 0 && self < other)
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

    /*
    // Check if the collision will occur during entire 0.0 -> delta_time interval
    if((TRUE == same_y_speed && y_overlap && (t_x_in < 0.0 && t_x_out > delta_time)) || // Overlapping y, and x entire time
       (TRUE == same_x_speed && x_overlap && (t_y_in < 0.0 && t_y_out > delta_time)) || // Overlapping x, and y entire time
       (FALSE == same_x_speed && FALSE == same_y_speed && t_x_in < 0.0 && t_x_out > delta_time && t_y_in < 0.0 && t_y_out > delta_time))         // Both x and y entire time
      {
        collision_data = CollisionData_new();
        collision_data->self = self;
        collision_data->other = other;
	collision_data->wayout = wayout_start;
	collision_data->time = 0.0;
	collision_data->type = COLLISION_PERSIST;
	LinkedList_addLast(collisions, collision_data);

	return TRUE;
      }

    // Check if the collision will occur during entire 0.0 -> delta_time interval
    if((TRUE == same_y_speed && y_overlap && t_x_in >= 0.0 && t_x_in < delta_time) || // Overlapping y, and x enters after 0 and enters before 1
       (TRUE == same_x_speed && x_overlap && t_y_in >= 0.0 && t_y_in < delta_time) || // Overlapping x, and y enters after 0 and enters before 1
       (FALSE == same_x_speed && FALSE == same_y_speed && t_x_in >= 0.0  && t_x_in < delta_time && t_y_in <= t_x_in) ||       // Y enters before X, and X enters after 0 and before 1
       (FALSE == same_x_speed && FALSE == same_y_speed && t_y_in >= 0.0  && t_y_in < delta_time && t_x_in <= t_y_in))         // X enters before Y, and Y enters after 0 and before 1
      {
        collision_data = CollisionData_new();
        collision_data->self = self;
        collision_data->other = other;
	collision_data->type = COLLISION_ENTER;

	// If X-axis enters before Y-axis
	if((FALSE == same_x_speed && FALSE == same_y_speed && t_x_in > t_y_in) || TRUE == same_y_speed) {
	  collision_data->time =  t_x_in;
	  collision_data->normal = Vector_create2d( (self_x_start - other_x_start) / fabs(self_x_start - other_x_start), 0.0f);
	} else {
	  collision_data->time =  t_y_in;
	  collision_data->normal = Vector_create2d(0.0f, (self_y_start - other_y_start) / fabs(self_y_start - other_y_start));
	}

	float persist_time = collision_data->time;
	LinkedList_addLast(collisions, collision_data);

	// Add an persistent collision after enter
        collision_data = CollisionData_new();
        collision_data->self = self;
        collision_data->other = other;
	collision_data->wayout = wayout_end;
	collision_data->type = COLLISION_PERSIST;
	collision_data->time = persist_time;
	LinkedList_addLast(collisions, collision_data);
      }

    // Check if the collision will end during the interval 0.0 -> delta_time
    if((TRUE == same_y_speed && y_overlap && (t_x_out >= 0.0 && t_x_out < delta_time)) || // Overlapping y, and x exits after 0 and exits before 1
       (TRUE == same_x_speed && x_overlap && (t_y_out >= 0.0 && t_y_out < delta_time)) || // Overlapping x, and y exits after 0 and exits before 1
       (FALSE == same_x_speed && FALSE == same_y_speed && t_x_out >= 0.0 && t_x_out < delta_time && t_y_out >= t_x_out) ||                  // X exits before Y, and X exits after 0 and before 1
       (FALSE == same_x_speed && FALSE == same_y_speed && t_y_out >= 0.0 && t_y_out < delta_time && t_x_out >= t_y_out))                    // Y exits before X, and Y exits after 0 and before 1
      {
	// Add an persistent collision before exit
        collision_data = CollisionData_new();
        collision_data->self = self;
        collision_data->other = other;
	collision_data->wayout = wayout_start;
	collision_data->type = COLLISION_PERSIST;
	collision_data->time = 0.0;
	LinkedList_addLast(collisions, collision_data);

        collision_data = CollisionData_new();
        collision_data->self = self;
        collision_data->other = other;
	collision_data->type = COLLISION_EXIT;

	// If X-axis exits before Y-axis
	if((FALSE == same_x_speed && FALSE == same_y_speed && t_x_out < t_y_out) || TRUE == same_y_speed) {
	  collision_data->time =  t_x_out;
	  collision_data->normal = Vector_create2d( (self_x_start - other_x_start) / fabs(self_x_start - other_x_start), 0.0f);
	} else {
	  collision_data->time =  t_y_out;
	  collision_data->normal = Vector_create2d(0.0f, (self_y_start - other_y_start) / fabs(self_y_start - other_y_start));
	}

	LinkedList_addLast(collisions, collision_data);
      }

    if(NULL == collisions->first) 
      {
	return FALSE;
      } 
    else
      {
	return TRUE;
      }*/
}


static BOOL _detectCollisions(LinkedList* self, LinkedList* other, float delta_time, Map* collisions)
{
    LinkedList* new_collisions = LinkedList_new();

    Node* self_node;
    Node* other_node;

    Point self_start;
    Point self_end;
    Point other_start;
    Point other_end;

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
					
					_addCollision(collisions, self_body, data);
					
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


BOOL _default_handleEnterCollision(void* lparam, void* rparam)
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

  Vector velocity_other_n = Vector_projection(other_velocity, data->normal);

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

BOOL _default_handleInCollision(void* lparam, void* rparam)
{
  CollisionData* data = (CollisionData*) rparam;
  float length = Vector_length(data->wayout);
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

      if(Point_getX(&p) != Point_getX(&wp->point) || Point_getY(&p) != Point_getY(&wp->point))
	{
	  result = TRUE;
	}
    }

  return result;
}

BOOL _default_handleExitCollision(void* lparam, void* rparam)
{
  CollisionData* data = (CollisionData*) rparam;
  //DEBUG("COLLISION EXIT: %f", data->time);
  return FALSE;
}

static BOOL _handleCollisions(Map* collisions, Map* handlers)
{
  /*
  MapNode* top;
  MapNode* node;
  MapNode* hook_node;
  _CollisionHookType type;
  Hook* hook;
  Map* hooks;
  CollisionData* data;
  BOOL changes_made = FALSE;

  //  DEBUG("HANDLING");
  for(top = collisions->first; top != NULL; top = top->next)
    {
      Map* body_collisions = (Map*) top->value;
      
      for(node = body_collisions->first; node != NULL; node = node->next)
	{
	  data = (CollisionData*) node->key;
	  
	  type.self = data->self->type;
	  type.other = data->other->type;

	  hooks = (Map*) Map_get(handlers, &type);
	  hook = (Hook*) Map_get(hooks, (void*) data->type);

	  if(TRUE == Hook_call(hook, (void*) data))
	    {
	      _clearCollisionsAfterTime(body_collisions, data->absoluteTime);
	      changes_made = TRUE;
	      break;
	    }
	}
    }

  return changes_made;
  */
}

static BOOL _updateCollisions(Map* collisions)
{
  return FALSE;
}

static BOOL _addCollision(Map* collisions, Body* self, CollisionData* collision_data)
{  
  Map* body_collisions = Map_get(collisions, self);

  if(NULL == body_collisions)
    {
      body_collisions = Map_new();
      body_collisions->compareKeys = _compareCollisionData;
      Map_set(collisions, self, body_collisions);
    }
  
  Map_set(body_collisions, collision_data, NULL);
}

static BOOL _removeCollision(Map* collisions, Body* self, Body* other)
{  
  return FALSE;
}

static BOOL _inCollision(Map* collisions, Body* self, Body* other)
{
  return FALSE;
}

static void _clearCollisionsAfterTime(Map* collisions, float time)
{
  /*  MapNode* node = collisions->first;
  MapNode* prev_node = node;
  CollisionData* data;

  while(node != NULL)
    {      
      data = (CollisionData*) node->key;

      if(data->absoluteTime >= time)
	{
	  node = NULL;
	}
      else
	{
	  prev_node = node;
	  node = node->next;
	}
    }

  if(NULL != prev_node)
    {
      while(prev_node->next != NULL)
	{
	  data = (CollisionData*) prev_node->next->key;
	  
	  Map_remove(collisions, (void*) data);
	  CollisionData_delete(data);
	}      
    }
  */
}

static void _clearCollisions(Map* collisions)
{
  /*
  Body* body;
  Map* body_collisions;
  CollisionData* data;
  
  while(collisions->first != NULL)
    {
      body = (Body*) collisions->first->key;
      body_collisions = (Map*) collisions->first->value;

      while(body_collisions->first != NULL)
	{
	  data = (CollisionData*) body_collisions->first->key;
	  Map_remove(body_collisions, (void*) data);
	  CollisionData_delete(data);
	}

      Map_remove(collisions, (void*) body);
    }
  */
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
    assert(!isInitialized);

    float t;
    body_map = Map_new();

    collision_hooks = Map_new();
    collision_hooks->compareKeys = _compareCollisionHookType;

    collision_list = Map_new();
    collision_types = Map_new();

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
    assert(isInitialized);
    MapNode* node;

    /*
    for(node = body_map->first; node != NULL; node = node->next)
        {
            LinkedList_delete((LinkedList*) node->value);
        }
    */
    Map_delete(collision_list);
    Map_delete(body_map);
    Map_delete(collision_hooks);
    Map_delete(collision_types);
    
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


    // Add the collision type to the list of types
    
    LinkedList* types = Map_get(collision_types, (void*) self);

    if(NULL == types)
      {
	types = LinkedList_new();
	Map_set(collision_types, (void*) self, types);
      }

    Node* node;
    BOOL found = FALSE;

    for(node = types->first; node != NULL; node = node->next)
      {
	if( *((BODY_TYPE*)node->item) == other)
	  {
	    found = TRUE;
	  }
      }

    if(FALSE == found)
      {
	BODY_TYPE* new_type = malloc(sizeof(BODY_TYPE));
	*new_type = other;
	LinkedList_addLast(types, (void*) new_type);
      }
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

void Physics_update(TIME time, BOOL do_update)
{
    MapNode* node;

    if(_last_time < 0.0)
      {
	_last_time = time;
      }

    float delta_time = time - _last_time;
    _last_time = time;

    delta_time *= 10.0;

    /*
    for(node = body_map->first; node != NULL; node = node->next)
        {
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


