#include <utils/pair.h>
#include <smugstd.h>

Pair* Pair_new()
{
  Pair* pair;
  pair = malloc(sizeof(Pair));
  return pair;
}

void Pair_delete(Pair* pair)
{
  free(pair);
}
