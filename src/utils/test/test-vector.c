#include <stdio.h>

#include "vector.h"

int main()
{
  Vector u, v, w;

  v = Vector_create2d(1, 1);
  w = Vector_create2d(2, 3);

  u = Vector_add(v, w);

  if(u.d[0] != 3 || u.d[1] != 4) {
    printf("Vector addition failed.\n");
    return 1;
  }

  return 0;
}
