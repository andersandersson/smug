/** @file calc.h
  * @brief Calc calculations for the physics
  * @ingroup smug_physics
  */

/** @defgroup smug_physics Calc
  *
  * @{
  */

#ifndef SMUG_PHYSICS_CALC_H
#define SMUG_PHYSICS_CALC_H

#include <common/common.h>
#include <utils/vector_type.h>
#include <utils/point_type.h>

int _collideInterval1D(float x1_start, float x1_width, float x2_start, float x2_width, float *width);
int _collideMovingPoints1D(float x1, float v1, float a1, float x2, float v2, float a2, float *t1, float* t2);
int _collideMovingInterval1D(float i1_start, float i1_vel, float i1_acc, float i1_width,
			     float i2_start, float i2_vel, float i2_acc, float i2_width,
			     float* t1_in, float* t1_out, float* t2_int, float* t2_out);

Point calculate_position(Point start, Vector velocity, Vector acceleration, TIME time);
Vector calculate_velocity(Vector velocity, Vector acceleration, TIME time);



#endif /* SMUG_PHYSICS_CALC_H */

/**@}*/
