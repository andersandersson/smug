#include "vector.h"

#include <stdio.h>
#include <math.h>

Vector Vector_create2d(double x, double y)
{
  Vector v;

  v.size = 2;

  v.x[0] = x;
  v.x[1] = y;

  return v;
}


Vector Vector_crossProduct2d(Vector v)
{
  return Vector_create2d(-v.x[1], v.x[0]);
}


Vector Vector_add(Vector v, Vector w)
{
  int i;
  Vector u;
  u.size = v.size;

  for(i=0; i < v.size; i++)
    {
      u.x[i] = v.x[i] + w.x[i];
    }

  return u;
}


Vector Vector_sub(Vector v, Vector w)
{
  int i;
  Vector u;
  u.size = v.size;

  for(i=0; i < v.size; i++)
    {
      u.x[i] = v.x[i] - w.x[i];
    }

  return u;
}


Vector Vector_multiply(Vector v, double k)
{
  int i;
  Vector u;
  u.size = v.size;

  for(i=0; i < v.size; i++)
    {
      u.x[i] = k*v.x[i];
    }

  return u;
}


double Vector_dotProduct(Vector v, Vector w)
{
  int i;
  double m = 0;

  for(i=0; i < v.size; i++)
    {
      m += v.x[i] * w.x[i];
    }

  return m;
}


Vector Vector_normalize(Vector v)
{
  Vector u;
  int i;
  double size = sqrt(Vector_DotProduct(v, v));

  u.size = v.size;
  
  for(i=0; i < v.size; i++) 
    {
      u.x[i] = v.x[i] / size;
    }

  return u;
}


Vector Vector_projection(Vector v, Vector n)
{
  return Vector_sub(v, Vector_multiply(n, Vector_DotProduct(v, n)));
}

void Vector_print(Vector v)
{
  int i;

  fprintf(stderr, "[");
  
  for(i=0; i < v.size; i++) {
    fprintf(stderr, "%.2f", v.x[i]);

    if(v.size - i > 1) {
      fprintf(stderr, ", ");
    }
  }

  fprintf(stderr, "]\n");
}
