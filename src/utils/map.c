#include <smugstd.h>
#include <utils/map.h>
#include <utils/pair.h>
#include <utils/binarysearchtree.h>

/** Comparison function for maps
 *
 * Takes the comparison arguments, pick out the key parts
 * of these pairs, and send them to the maps comparison
 * function that is specified by the user of the map.
 *
 * @relatesalso Map
 * @param map Pointer to the Map containing the tree
 * @param left The left Pair
 * @param right The right Pair
 * @return -1, 0 or 1
 */
static int _pair_compare(void* map, void* left, void* right)
{
    Pair* _left = (Pair*) left;
    Pair* _right = (Pair*) right;
    Map* _map = (Map*) map;

    return _map->compare(map, _left->left, _right->left);
}

/** Default comparison function for maps
 *
 * Simply compare the pointer addresses
 *
 * @relatesalso Map
 * @param self Ignored
 * @param left The left key
 * @param right The right key
 * @return -1, 0 or 1
 */
static int _default_compare(void* self, void* left, void* right)
{
    if(left < right)
    {
        return -1;
    }
    else if(left > right)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

Map* Map_new(void)
{
    Map* map = malloc(sizeof(Map));

    Map_init(map);

    return map;
}


void Map_init(Map* map)
{
    BinarySearchTree_init((BinarySearchTree*) map);
    BinarySearchTree_setCompare((BinarySearchTree*) map, map, _pair_compare);

    map->compare = _default_compare;
}


void Map_delete(Map* map)
{
    free(map);
}


void Map_setCompare(Map* map, int (*compare)(void*, void*, void*))
{
    map->compare = compare;
    BinarySearchTree_setCompare((BinarySearchTree*) map, map, _pair_compare);
}

void Map_set(Map* map, void* key, void* value)
{
    Pair* pair = Pair_new();

    pair->left = key;
    pair->right = value;

    BinarySearchTree_insert((BinarySearchTree*) map, pair);
}

Pair Map_remove(Map* map, void* key)
{
    Pair search;

    search.left = key;

    // Search and store the element to be removed
    Pair* found = BinarySearchTree_remove((BinarySearchTree*) map, &search);

    Pair pair;

    if(NULL == found)
    {
        pair.left = NULL;
        pair.right = NULL;
    }
    else
    {
        // Store the key and value pointer addresses
        pair.left = found->left;
        pair.right = found->right;
    }

    Pair_delete(found);

    return pair;
}

void* Map_get(Map* map, void* key)
{
    Pair search;
    search.left = key;

    Pair* pair = BinarySearchTree_find((BinarySearchTree*) map, &search);

    if(NULL == pair)
    {
        return NULL;
    }

    return pair->right;
}



MapIterator* MapIterator_new(void)
{
    MapIterator* iter = malloc(sizeof(MapIterator));

    MapIterator_init(iter);

    return iter;
}


void MapIterator_init(MapIterator* iter)
{
    BinarySearchTreeIterator_init((BinarySearchTreeIterator*) iter);
}


void MapIterator_delete(MapIterator* iter)
{
    if(NULL == iter)
    {
        return;
    }

    BinarySearchTreeIterator_delete((BinarySearchTreeIterator*) iter);

    free(iter);
}


void MapIterator_reset(Map* map, MapIterator* iter)
{
    BinarySearchTreeIterator_reset((BinarySearchTree*) map, (BinarySearchTreeIterator*) iter);
}


void MapIterator_step(MapIterator* iter)
{
    BinarySearchTreeIterator_step((BinarySearchTreeIterator*) iter);
}


BOOL MapIterator_valid(MapIterator* iter)
{
    return BinarySearchTreeIterator_valid((BinarySearchTreeIterator*) iter);
}


void* MapIterator_get(MapIterator* iter)
{
    return BinarySearchTreeIterator_get((BinarySearchTreeIterator*) iter);
}


