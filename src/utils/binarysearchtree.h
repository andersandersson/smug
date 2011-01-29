/** @file binarysearchtree.h
 * @brief Defines a binary search tree type, node and iterator
 * @ingroup smug_utils
 */

/** @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_BINARYSEARCHTREE_H
#define SMUG_UTILS_BINARYSEARCHTREE_H

#include <common/common.h>
#include <utils/linkedlist.h>

/* Forward declaration */
struct BinarySearchTreeNode;

/** A binary search tree struct
 *
 * Pretty much a holder of the root node of the tree.
 * Also has a compare function to use when building the tree.
 * This function will take three parameters, with the first
 * parameter chosen by the user. This will make it somewhat
 * possible to "override" the compare function in other
 * structs.
 *
 * @sa ::BinarySearchTreeNode
 */
typedef struct BinarySearchTree
{
    struct BinarySearchTreeNode* root; /**< The root node of the tree */

    void* compareData; /**< Data to be sent as first parameter to compare */
    int (*compare)(void*, void*, void*); /**< Compare function to use when comparing two node elements */
} BinarySearchTree;


/** An iterator struct for binary search trees
 *
 * Struct used to iterate over a tree. The user should not
 * have knowledge about the implementation of the tree.
 *
 * @sa ::BinarySearchTree, ::BinarySearchTreeNode
 */
typedef struct BinarySearchTreeIterator
{
    struct BinarySearchTreeNode* cursor; /**< Pointer to current node */
} BinarySearchTreeIterator;


/** Creates and returns a new BinarySearchTree pointer
 *
 * @relatesalso BinarySearchTree
 * @return Pointer to the newly created tree
 */
BinarySearchTree* BinarySearchTree_new(void);


/** Initialize a BinarySearchTree
 *
 * @relatesalso BinarySearchTree
 * @param tree The tree to init
 */
void BinarySearchTree_init(BinarySearchTree* tree);


/** Destroy a tree
 *
 * Free the memory used by a tree. The content, however, must
 * be handled by the user.
 *
 * @relatesalso BinarySearchTree
 * @param tree A pointer to the tree to be deleted
 */
void BinarySearchTree_delete(BinarySearchTree* tree);


/** Set comparison function for the tree
 *
 * The default behaviour is to compare the pointer addresses
 * of the elements. Override this to make your own comparison.
 * The first parameter in the compare function will be the value
 * sent to data her.
 *
 * @relatesalso BinarySearchTree
 * @param tree    The tree to apply comparison to
 * @param data    The first argument to be sent to compare
 * @param compare The comparison function. Should return -1, 0 or 1 for lesser than, equal or greater than.
 */
void BinarySearchTree_setCompare(BinarySearchTree* tree, void* data, int (*compare)(void*, void*, void*));


/** Insert an element into the tree
 *
 * @relatesalso BinarySearchTree
 * @param tree    The tree to insert into
 * @param element The element to insert
 * @return Whether the element was inserted or not
 */
BOOL BinarySearchTree_insert(BinarySearchTree* tree, void* element);


/** Remove an element from the tree
 *
 * This function will take the element that matches
 * according to the comparison function and remove
 * it.
 *
 * @relatesalso BinarySearchTree
 * @param tree    The tree to remove from
 * @param element The element to remove
 * @return A pointer to the element in the removed node
 */
void* BinarySearchTree_remove(BinarySearchTree* tree, void* element);


/** Conditional remove of element from tree
 *
 * This function will iterate over the tree and remove
 * all elements that will match the predicate.
 *
 * @relatesalso BinarySearchTree
 * @param tree    The tree to remove from
 * @param predicate The predicate to apply
 * @param param The second parameter to the predicate function
 * @return A pointer to a list of removed elements
 */
LinkedList* BinarySearchTree_removeIf(BinarySearchTree* tree, BOOL (*predicate)(void*, void*), void* param);


/** Find an element in the tree
 *
 * Find the given element in the tree according to the
 * comparison function. If no element is found, NULL is
 * returned.
 *
 * @relatesalso BinarySearchTree
 * @param tree    The tree to search
 * @param element The element to search for
 * @return The found element or NULL
 */
void* BinarySearchTree_find(BinarySearchTree* tree, void* element);


/** Find a list of elements in the tree matching predicate
 *
 * Find a list of elements in the tree that matches the given
 * predicate. The predicate is a binary function whose first argument
 * will be the data paremeter sent in to findIf.
 *
 * @relatesalso BinarySearchTree
 * @param tree    The tree to search
 * @param predicate The binary predicate function
 * @param param The first parameter to the predicate function
 * @return A list of all found elements
 */
LinkedList* BinarySearchTree_findIf(BinarySearchTree* tree, BOOL (*predicate)(void*, void*), void* param);


/** Find the minimum element in the tree
 *
 * Get the leftmost item in the tree.
 *
 * @relatesalso BinarySearchTree
 * @param tree The tree to search
 * @return The found item or NULL
 */
void* BinarySearchTree_findMin(BinarySearchTree* tree);


/** Print a tree to stdout
 *
 * This is mostly for debugging purpose.
 *
 * @relatesalso BinarySearchTree
 * @param tree  The tree to print
 */
void BinarySearchTree_print(BinarySearchTree* tree);


/** Creates and returns a BinarySearchTreeIterator
 *
 * Create an iterator used to iterate a tree.
 *
 * @relatesalso BinarySearchTreeIterator
 * @sa ::BinarySearchTreeIterator_reset
 * @return The created iterator
 */
BinarySearchTreeIterator* BinarySearchTreeIterator_new(void);


/** Initializes an iterator
 *
 * Init all values on an iterator
 *
 * @relatesalso BinarySearchTreeIterator
 * @sa ::BinarySearchTreeIterator_reset
 * @param iter The iterator to initialize
 */
void BinarySearchTreeIterator_init(BinarySearchTreeIterator* iter);


/** Destroy a BinarySearchTreeIterator
 *
 * Free the memory used by the iterator.
 *
 * @relatesalso BinarySearchTreeIterator
 * @param iter The iterator to delete
 */
void BinarySearchTreeIterator_delete(BinarySearchTreeIterator* iter);


/** Reset the iterator as a forward iterator
 *
 * Set the iterator at the beginning (the least element)
 * of the tree.
 *
 * @relatesalso BinarySearchTreeIterator
 * @param tree The tree to iterate through
 * @param iter The iterator to connect
 */
void BinarySearchTreeIterator_reset(BinarySearchTree* tree, BinarySearchTreeIterator* iter);


/** Step the iterator forward in the tree
 *
 * Move the iterator to the next node in the tree
 *
 * @relatesalso BinarySearchTreeIterator
 * @param iter The iterator to step
 */
void BinarySearchTreeIterator_step(BinarySearchTreeIterator* iter);


/** Check if the iterator is pointing at a valid item
 *
 * Will check that the iterator is currently pointing
 * at a node in the tree. Will return falls if, for example,
 * the iterator has reached the end of the tree.
 *
 * @relatesalso BinarySearchTreeIterator
 * @param iter The iterator to check
 * @return TRUE if the iterator is pointing on a node, FALSE otherwise
 */
BOOL BinarySearchTreeIterator_valid(BinarySearchTreeIterator* iter);


/** Get the item at the current position
 *
 * Get the element that the iterator is currently pointing at.
 * The user should make sure that the iterator is in a valid
 * state before calling this.
 *
 * @sa ::BinarySearchTreeIterator_valid
 * @relatesalso BinarySearchTreeIterator
 * @param iter The iterator to check
 * @return The element at the current position or NULL if invalid
 */
void* BinarySearchTreeIterator_get(BinarySearchTreeIterator* iter);


#endif // SMUG_UTILS_BINARYSEARCHTREE_H

/**@}*/
