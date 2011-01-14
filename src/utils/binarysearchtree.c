#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>

#include <utils/binarysearchtree.h>


static int _default_compare(void* left, void* right);
static void _insert_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element, int height);
static void _remove_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element);
static BinarySearchTreeNode* _find_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element);
static BinarySearchTreeNode* _find_min_node(BinarySearchTreeNode* node);
static void _print_tree(BinarySearchTreeNode* node, int level);
static BinarySearchTreeNode* _find_unvisited_above(BinarySearchTreeNode* parent, BinarySearchTreeNode* right);

int _default_compare(void* left, void* right)
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


void BinarySearchTreeNode_copy(BinarySearchTreeNode* self, BinarySearchTreeNode* other)
{
    self->left = other->left;
    self->right = other->right;
    self->parent = other->parent;
    self->element = other->element;
    self->height = other->height;
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


void BinarySearchTree_setCompare(BinarySearchTree* tree, int (*compare)(void*, void*))
{
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
  int side = tree->compare(element, node->element);

  if(0 == side)
    {
    }
  else if(-1 == side)
    {
      if(NULL == node->left)
	{
	  if(NULL == node->right)
	    {
	      node->height = 1;
	    }

	  node->left = BinarySearchTreeNode_new();
	  node->left->element = element;
	  node->left->parent = node;
	}
      else
	{
	  _insert_node(tree, node->left, element, height + 1);
	}
    }
  else if(1 == side)
    {
      if(NULL == node->right)
	{
	  if(NULL == node->left)
	    {
	      node->height = 1;
	    }

	  node->right = BinarySearchTreeNode_new();
	  node->right->element = element;
	  node->right->parent = node;
	}
      else
	{
	  _insert_node(tree, node->right, element, height + 1);
	}
    }
  
  int left_height = (node->left == NULL) ? 0 : node->left->height+1;
  int right_height = (node->right == NULL) ? 0 : node->right->height+1;

  node->height = max(left_height, right_height);
}


void BinarySearchTree_remove(BinarySearchTree* tree, void* element)
{
  BinarySearchTreeNode* node = _find_node(tree, tree->root, element);

  if(NULL == node)
    {
      return;
    }
  else
    {
      _remove_node(tree, tree->root, element);
    }
}

void _remove_node(BinarySearchTree* tree, BinarySearchTreeNode* node, void* element)
{
  if(node == NULL)
    {
      return;
    }

  int side = tree->compare(element, node->element);

  if(-1 == side)
    {      
      _remove_node(tree, node->left, element);
    }
  else if(1 == side)
    {
      _remove_node(tree, node->right, element);
    }
  else if(0 == side)
    {
      if(node->left != NULL && node->right != NULL)
	{
	  BinarySearchTreeNode* tmp = _find_min_node(node->right);
	  BinarySearchTreeNode* right = node->right;
	  
	  node->element = tmp->element;

	  _remove_node(tree, right, tmp->element);
	}
      else
	{
	  if(node->left == NULL && node->right != NULL)
	    {
	      BinarySearchTreeNode_copy(node, node->right);
	      BinarySearchTreeNode_delete(node->right);
	    }
	  else if(node->right == NULL && node->left != NULL)
	    {
	      BinarySearchTreeNode_copy(node, node->left);
	      BinarySearchTreeNode_delete(node->left);
	    }
	  else {
	    if(node->parent != NULL && node == node->parent->left)
	      {
		node->parent->left = NULL;
		BinarySearchTreeNode_delete(node);
	      }
	    else if(node->parent != NULL && node == node->parent->right)
	      {
		node->parent->right = NULL;
		BinarySearchTreeNode_delete(node);
	      }
	  }
	}
    }

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

  int side = tree->compare(element, node->element);

  if(-1 == side)
    {
      return _find_node(tree, node->left, element);
    }
  else if(1 == side)
    {
      return _find_node(tree, node->right, element);
    }
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

  if(NULL != cursor->right)
    {
      iter->cursor = _find_min_node(cursor->right);
    }
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

  if(right == parent->right)
    {
      return _find_unvisited_above(parent->parent, parent);
    }

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
