#ifndef SMUG_UTILS_PAIR_H
#define SMUG_UTILS_PAIR_H

typedef struct Pair
{
  void* left;
  void* right;
} Pair;

Pair* Pair_new(void);
void Pair_delete(Pair* pair);

#endif // SMUG_UTILS_PAIR_H
