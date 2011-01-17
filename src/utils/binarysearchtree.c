#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>

#include <utils/binarysearchtree.h>

/** The node type building the tree
  * 
  * The building blocks of the tree. It holds a pointer to its
  * parent to make iteration easier.
  * 
  * @sa ::BinarySearchTree
  */
typedef struct BinarySearchTreeNode
{
  struct BinarySearchTreeNode* parent; /**< Pointer to parent, will make iteration easier */
  struct BinarySearchTreeNode* left;   /**< Pointer to the left child, the one considered "lesser" */
  struct BinarySearchTreeNode* right;  /**< Pointer to the right child, the one considered "greater" */
  
  void* element; /**< Pointer to the element of the node */

  int height;    /**< The height of the subtree of the node */
} BinarySearchTreeNode;


/** Create and return a new BinarySearchTreeNode
  *
  * @relatesalso BinarySearchTreeNode
  * @return The newly created BinarySearchTreeNode
  */
static BinarySearchTreeNode* BinarySearchTreeNode_new(void);


/** Destroy a BinarySearchTreeNode
  *
  * Free the memory from one single node 
  *
  * @relatesalso BinarySearchTreeNode
  * @param node The node to delete
  */
static void BinarySearchTreeNode_delete(BinarySearchTreeNode* node);


/** Delete a BinarySearchTreeNode recurively
  *
  * Free the memory of a node and all its children
  * and so on.
  *
  * @relatesalso BinarySearchTreeNode
  * @param node The node to delete
  */
static void BinarySearchTreeNode_deleteRecursive(BinarySearchTreeNode* node);


/** Copy a BinarySearchTreeNode to another 
  * 
  * @param src The source node to copy from
  * @param dest Where to copy to
  */
static void BinarySearchTreeNode_copy(BinarySearchTreeNode* src, BinarySearchTreeNode* dest);


/** Default comparison function
  *
  * This function simply compares the addresses of the pointers
  * given. The return values are:
  *
  * -1 if left < right
  *  0 if left == right
  *  1 if left > right
  *
  * @param self Pointer to the BinarySearchTree we are working in
  * @param left Left hand side of the comparison
  * @param right Right hand side of the comparison
  */
static int _default_compare(void* self, void* left, void* right);

 
/** Helper function for inserting nodes
  *
  * Inserts a node in the tree, under the given tree. The tree is
  * required since it has the compare function.
  *
  * @relatesalso BinarySearchTree
  * @param tree The tree the node belongs to
  * @param node The root node of the subtree to insert under
  * @param element The element to insert
  * @param height The current height of the subtree
  */
static void _insert_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element, int height);


/** Helper function for removing nodes
  * 
  * Remove a node from a given subtree.
  *
  * @relatesalso BinarySearchTree
  * @param tree The tree to remove from
  * @param node The root node for the subtree to remove from
  * @param element The element to remove
  */
static void _remove_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element);


/** Helper function for finding a node
  * 
  * Find a node in a given subtree.
  *
  * @relatesalso BinarySearchTree
  * @param tree The tree to search in
  * @param node The root node of the subtree to search in.
  * @param element The element we are searching for.
  * @return The found node or NULL
  */
static BinarySearchTreeNode* _find_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element);


/** Helper function for finding a minimum node
  * 
  * Find the minimum node in a given subtree.
  *
  * @relatesalso BinarySearchTree
  * @param tree The tree to search in
  * @param node The root node of the subtree to search in.
  * @return The found node or NULL
  */
static BinarySearchTreeNode* _find_min_node(BinarySearchTreeNode* node);


/** Helper function for printing a tree
  *
  * @relatesalso BinarySearchTree
  * @param node The root node of the subtree to print
  * @param level Indentation level
  */
static void _print_tree(BinarySearchTreeNode* node, int level);

/** Helper function for iterating a tree
  * 
  * Finds the least node above the current that
  * has yet not been iterated. 
  *
  * @relatesalso BinarySearchTreeIterator
  * @param tree The tree to search in
  * @param node The root node of the subtree to search in.
  * @return The found node or NULL
  */
static BinarySearchTreeNode* _find_unvisited_above(BinarySearchTreeNode* parent, BinarySearchTreeNode* right);


int _default_compare(void* self, void* left, void* right)
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


BinarySearchTreeNode* BinarySearchTreeNode_new(void)
{
    BinarySearchTreeNode* node = malloc(sizeof(BinarySearchTreeNode));

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->element = NULL;
    node->height = 0;

    return node;
}


void BinarySearchTreeNode_deleteRecursive(BinarySearchTreeNode* node)
{
  if(NULL == node)
    {
      return;
    }

  if(NULL != node->left)
    {
      BinarySearchTreeNode_delete(node->left);
    }

  if(NULL != node->right)
    {
      BinarySearchTreeNode_delete(node->right);
    }

  free(node);
}


void BinarySearchTreeNode_delete(BinarySearchTreeNode* node)
{
  if(NULL == node)
    {
      return;
    }

  free(node);
}


void BinarySearchTreeNode_copy(BinarySearchTreeNode* src, BinarySearchTreeNode* dest)
{
    dest->left = src->left;
    dest->right = src->right;
    dest->parent = src->parent;
    dest->element = src->element;
    dest->height = src->height;
}


BinarySearchTree* BinarySearchTree_new(void)
{
    BinarySearchTree* tree = malloc(sizeof(BinarySearchTree));

    tree->compare = _default_compare;

    tree->root = NULL;

    return tree;
}


void BinarySearchTree_delete(BinarySearchTree* tree)
{
    BinarySearchTreeNode_deleteRecursive(tree->root);
    free(tree);
}


void BinarySearchTree_setCompare(BinarySearchTree* tree, void* data, int (*compare)(void*, void*, void*))
{
    tree->compareData = data;
    tree->compare = compare;
}


void BinarySearchTree_insert(BinarySearchTree* tree, void* element)
{
  if(tree->root == NULL)
    {
      tree->root = BinarySearchTreeNode_new();
      tree->root->element = element;
    }
  else
    {
      _insert_node(tree, tree->root, element, 0);
    }
}

void _insert_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element, int height)
{
  // Check the relation between the new element and the current
  int side = tree->compare(tree->compareData, element, node->element);

  // They are equal, do nothing
  if(0 == side)
    {
    }
  // It is to the left
  else if(-1 == side)
    {
      if(NULL == node->left)
	{
	  // The current node has no childs, so set its new height to 1
	  if(NULL == node->right)
	    {
	      node->height = 1;
	    }

	  node->left = BinarySearchTreeNode_new();
	  node->left->element = element;
	  node->left->parent = node;
	}
      // Otherwise, insert the node to the left subtree
      else
	{
	  _insert_node(tree, node->left, element, height + 1);
	}
    }
  // It is to the right
  else if(1 == side)
    {
      if(NULL == node->right)
	{
	  // The current node has no childs, so set its new height to 1
	  if(NULL == node->left)
	    {
	      node->height = 1;
	    }

	  node->right = BinarySearchTreeNode_new();
	  node->right->element = element;
	  node->right->parent = node;
	}
      // Otherwise, insert the node to the right subtree
      else
	{
	  _insert_node(tree, node->right, element, height + 1);
	}
    }

  // Check the heights of the right and left subtree
  int left_height = (node->left == NULL) ? 0 : node->left->height+1;
  int right_height = (node->right == NULL) ? 0 : node->right->height+1;

  // Set currents node new height
  node->height = max(left_height, right_height);
}


void* BinarySearchTree_remove(BinarySearchTree* tree, void* element)
{
  BinarySearchTreeNode* node = _find_node(tree, tree->root, element);
  void* _element;

  // If the element does not exist in the tree, do nothing and return NULL
  if(NULL == node)
    {
      return NULL;
    }
  // Otherwise, save the element for returning, and remove the 
  // element from the tree.
  else
    {
      _element = node->element;
      _remove_node(tree, tree->root, element);
    }

  return _element;
}

void _remove_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element)
{
  // If we are trying to remove NULL, return
  if(node == NULL)
    {
      return;
    }

  // Check the elements relation to the current
  int side = tree->compare(tree->compareData, element, node->element);

  // It is to the left, recurse subtree
  if(-1 == side)
    {      
      _remove_node(tree, node->left, element);
    }
  // It is to the right, recurse subtree
  else if(1 == side)
    {
      _remove_node(tree, node->right, element);
    }
  // It is equal, remove
  else if(0 == side)
    {
      // If the node has children in both left and right subtree
      if(node->left != NULL && node->right != NULL)
	{
	  // Get the min node from the right subtree
	  BinarySearchTreeNode* tmp = _find_min_node(node->right);
	  BinarySearchTreeNode* right = node->right;
	  
	  // Copy the minimum element from the right subtree to the current node
	  node->element = tmp->element;

	  // And remove the minimum element from the right subtree
	  _remove_node(tree, right, tmp->element);
	}
      else
	{
	  // If it has only a right subtree, copy the node below 
	  // the right and remove it from the subtree
	  if(node->left == NULL && node->right != NULL)
	    {
	      BinarySearchTreeNode_copy(node->right, node);
	      BinarySearchTreeNode_delete(node->right);
	    }
	  // If it has only a left subtree, copy the node below 
	  // the left and remove it from the subtree
	  else if(node->right == NULL && node->left != NULL)
	    {
	      BinarySearchTreeNode_copy(node->left, node);
	      BinarySearchTreeNode_delete(node->left);
	    }
	  // Otherwise, we are removing a single leaf
	  else {	    
	    // If the node is to the left of the parent
	    if(node->parent != NULL && node == node->parent->left)
	      {
		node->parent->left = NULL;
		BinarySearchTreeNode_delete(node);
	      }
	    // If the node is to the right of the parent
	    else if(node->parent != NULL && node == node->parent->right)
	      {
		node->parent->right = NULL;
		BinarySearchTreeNode_delete(node);
	      }
	  }
	}
    }

  // Calculate new height for the node
  int left_height = (node->left == NULL) ? 0 : node->left->height+1;
  int right_height = (node->right == NULL) ? 0 : node->right->height+1;
  node->height = max(left_height, right_height);
}


void* BinarySearchTree_find(BinarySearchTree* tree, void* element)
{
  BinarySearchTreeNode* node = _find_node(tree, tree->root, element);

  if(NULL != node)
    return node->element;

  return NULL;
}

BinarySearchTreeNode* _find_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element)
{
  if(node == NULL)
    {
      return NULL;
    }

  // Check elements relation
  int side = tree->compare(tree->compareData, element, node->element);

  // It is to the left
  if(-1 == side)
    {
      return _find_node(tree, node->left, element);
    }
  // It is to the right
  else if(1 == side)
    {
      return _find_node(tree, node->right, element);
    }
  // It is equal
  else
    {
      return node;
    }
}

BinarySearchTreeNode* _find_min_node(BinarySearchTreeNode* node)
{
  if(node == NULL)
    {
      return NULL;
    }

  if(node->left != NULL)
    {
      return _find_min_node(node->left);
    }

  return node;
}



void BinarySearchTree_print(BinarySearchTree* tree)
{
  if(NULL == tree || NULL == tree->root)
    {
      return;
    }

  _print_tree(tree->root, 0);
}

void _print_tree(BinarySearchTreeNode* node, int level)
{
  if(NULL != node)
    {
      _print_tree(node->left, level+1);
      int i;
      for(i=0; i<level; i++)
	printf("   ");
      printf(" /\n");
      
      for(i=0; i<level; i++)
	printf("   ");
      printf("%d[%d]\n", (int)node->element, node->height);
      
      for(i=0; i<level; i++)
	printf("   ");
      printf(" \\\n");

      _print_tree(node->right, level+1);
    }
}



BinarySearchTreeIterator* BinarySearchTreeIterator_new(void)
{
  BinarySearchTreeIterator* iter;
  iter = malloc(sizeof(BinarySearchTreeIterator));
  return iter;
}

void BinarySearchTreeIterator_delete(BinarySearchTreeIterator* iter)
{
  free(iter);
}

void BinarySearchTreeIterator_reset(BinarySearchTree* tree, BinarySearchTreeIterator* iter)
{
  BinarySearchTreeNode* node = _find_min_node(tree->root);

  iter->cursor = node;
}


void BinarySearchTreeIterator_step(BinarySearchTreeIterator* iter)
{
  if(NULL == iter->cursor)
    {
      return;
    }

  BinarySearchTreeNode* cursor = iter->cursor;

  // If we have just stepped up from a left node, find the
  // min to the right
  if(NULL != cursor->right)
    {
      iter->cursor = _find_min_node(cursor->right);
    }
  // Otherwise, step upwards
  else 
    {
      iter->cursor = _find_unvisited_above(cursor->parent, cursor);
    }
}

BinarySearchTreeNode* _find_unvisited_above(BinarySearchTreeNode* parent, BinarySearchTreeNode* right)
{
  if(NULL == parent)
    {
      return NULL;
    }

  // If we came from the right, keep moving up.
  if(right == parent->right)
    {
      return _find_unvisited_above(parent->parent, parent);
    }

  // Otherwise return the node
  return parent;
}



BOOL BinarySearchTreeIterator_valid(BinarySearchTreeIterator* iter)
{
  if(NULL == iter->cursor) 
    return FALSE;
  else
    return TRUE;
}


void* BinarySearchTreeIterator_get(BinarySearchTreeIterator* iter)
{
  return iter->cursor->element;
}
