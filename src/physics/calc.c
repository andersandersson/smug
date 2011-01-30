#include <physics/calc.h>
#include <common/common.h>
#include <common/log.h>
#include <utils/point.h>
#include <utils/vector.h>
#include <math.h>

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
	    *t2 = -p/2.0;
	    result = 2;
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
    else if(2 == num1 && 1 == num2)
    {
    }

    *t1_in = FP_NAN;
    *t1_out = FP_NAN;

    *t2_in = FP_NAN;
    *t2_out = FP_NAN;

    return 0;
}

Point calculate_position(Point start, Vector velocity, Vector acceleration, TIME time)
{
  Point result = Point_addVector(start, Vector_add(Vector_multiply(velocity, time),
						   Vector_multiply(acceleration, time*time*0.5)));

  return result;
}

Vector calculate_velocity(Vector velocity, Vector acceleration, TIME time)
{
  Vector result = Vector_add(velocity, Vector_multiply(acceleration, time));

  return result;
}

