#ifndef SMUG_UTILS_PAIR_H
#define SMUG_UTILS_PAIR_H

/** Simple Pair struct
  * 
  * Not much to say. It's a Pair.
  */
typedef struct Pair
{
  void* left;  //< 
  void* right; //< 
} Pair;


/** Create a new Pair
 *
 * @relatesalso Pair
 */
Pair* Pair_new(void);


/** Delete a Pair
 *
 * @relatesalso Pair
 * @param pair The Pair to be deleted
 */
void Pair_delete(Pair* pair);

#endif // SMUG_UTILS_PAIR_H
