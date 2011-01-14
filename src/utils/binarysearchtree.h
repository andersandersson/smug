#ifndef SMUG_UTILS_BINARYSEARCHTREE_H
#define SMUG_UTILS_BINARYSEARCHTREE_H

#include <common/common.h>

typedef struct BinarySearchTreeNode
{
    struct BinarySearchTreeNode* parent;
    struct BinarySearchTreeNode* left;
    struct BinarySearchTreeNode* right;

    void* element;

    int height;
} BinarySearchTreeNode;


typedef struct BinarySearchTree
{
    struct BinarySearchTreeNode* root;

    void* compareData;
    int (*compare)(void*, void*, void*);
} BinarySearchTree;


typedef struct BinarySearchTreeIterator 
{
    BinarySearchTreeNode* cursor;
} BinarySearchTreeIterator;


BinarySearchTreeNode* BinarySearchTreeNode_new(void);
void BinarySearchTreeNode_delete(BinarySearchTreeNode* node);
void BinarySearchTreeNode_deleteRecursive(BinarySearchTreeNode* node);
void BinarySearchTreeNode_copy(BinarySearchTreeNode* left, BinarySearchTreeNode* right);
BinarySearchTreeNode* BinarySearchTreeNode_findMin(BinarySearchTreeNode* node);
BinarySearchTreeNode* BinarySearchTreeNode_findMax(BinarySearchTreeNode* node);


BinarySearchTree* BinarySearchTree_new(void);
void BinarySearchTree_delete(BinarySearchTree* tree);
void BinarySearchTree_setCompare(BinarySearchTree* tree, void* data, int (*compare)(void*, void*, void*));
void BinarySearchTree_insert(BinarySearchTree* tree, void* element);
void* BinarySearchTree_remove(BinarySearchTree* tree, void* element);
void* BinarySearchTree_find(BinarySearchTree* tree, void* element);
void BinarySearchTree_print(BinarySearchTree* tree);


BinarySearchTreeIterator* BinarySearchTreeIterator_new(void);
void BinarySearchTreeIterator_delete(BinarySearchTreeIterator* iter);
void BinarySearchTreeIterator_reset(BinarySearchTree* tree, BinarySearchTreeIterator* iter);
void BinarySearchTreeIterator_step(BinarySearchTreeIterator* iter);
BOOL BinarySearchTreeIterator_valid(BinarySearchTreeIterator* iter);
void* BinarySearchTreeIterator_get(BinarySearchTreeIterator* iter);


#endif // SMUG_UTILS_BINARYSEARCHTREE_H
