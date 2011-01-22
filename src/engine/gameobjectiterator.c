#include <smugstd.h>
#include <common/common.h>
#include <utils/linkedlist.h>
#include <utils/linkedlistiterator.h>

#include <engine/gameobjectiterator.h>
#include <engine/gameobject.h>

GameObjectIterator* GameObject_getIterator(GameObject* self, int types)
{
    GameObjectIterator* iter = (GameObjectIterator*)malloc(sizeof(GameObjectIterator));
    iter->mChildIterators = LinkedList_new();
    iter->mTypes = types;
    iter->mHasReturnedRoot = FALSE;

    iter->mRootList = LinkedList_new();
    LinkedList_addFirst(iter->mRootList, self);
    LinkedList_addFirst(iter->mChildIterators, LinkedList_getIterator(iter->mRootList));

    return iter;
}

GameObject* GameObjectIterator_getNext(GameObjectIterator* self)
{
    LinkedListIterator* iter = (LinkedListIterator*)LinkedList_getFirst(self->mChildIterators);
    if (!self->mHasReturnedRoot)
    {
        // First time:
        self->mHasReturnedRoot = TRUE;
		// smug_printf("GameObjectIterator returning root object.");
        return LinkedListIterator_getNext(iter);
    }
    GameObject* obj = (GameObject*)LinkedListIterator_getSame(iter);
    // Check for more child objects:
    if (GameObject_hasChildObjects(obj))
    {
		// smug_printf("GameObjectIterator returning child object of last object.");
        LinkedListIterator* childIterator = GameObject_getChildIterator(obj);
        LinkedList_addFirst(self->mChildIterators, childIterator);
        return LinkedListIterator_getNext(childIterator);
    }
    // Check for more siblings, or siblings of parents and grandparents:
	int levels = 0;
    while (!LinkedListIterator_hasMore(iter))
    {
		levels++;
        LinkedList_popFirst(self->mChildIterators);
        if (LinkedList_isEmpty(self->mChildIterators))
        {
			// smug_printf("GameObjectIterator has reached the end.");
            return NULL;
        }
        iter = LinkedList_getFirst(self->mChildIterators);
    }
	// if (levels == 0)
		// smug_printf("GameObjectIterator returning sibling of last object.");
	// else
		// smug_printf("GameObjectIterator returning ancestor of last object.");
    return (GameObject*)LinkedListIterator_getNext(iter);
}

// GameObject* GameObjectIterator_getNext(GameObjectIterator* self)
// {
    // // First time:
    // if (LinkedList_isEmpty(self->mChildIterators))
    // {
        // LinkedList_addFirst(
            // self->mChildIterators,
            // GameObject_getChildIterator(self));
        // return self->mRoot;
    // }
    // LinkedListIterator* iter = (LinkedListIterator*)LinkedList_getFirst(self->mChildIterators);
    // GameObject* obj = (GameObject*)LinkedListIterator_getSame(iter);
    // // Check for more child objects:
    // if (GameObject_hasChildObjects(obj))
    // {
        // LinkedListIterator* childIterator;
        // childIterator = GameObject_getChildIterator(obj);
        // LinkedList_addFirst(self->mChildIterators, childIterator);
        // return LinkedListIterator_getNext(childIterator);
    // }
    // // Check for more siblings, or siblings of parents and grandparents:
    // while (!LinkedListIterator_hasMore(iter))
    // {
        // if (LinkedList_isEmpty(self->mChildIterators)
        // {
            // return NULL;
        // }
        // iter = LinkedList_popFirst(self->mChildIterators);
    // }
    // return (GameObject*)LinkedListIterator_getNext(iter);
// }

BOOL GameObjectIterator_hasMore(GameObjectIterator* self)
{
    if (!self->mHasReturnedRoot)
    {
        // Hasn't started iterating yet.
		// smug_printf("GameObjectIterator has not started iterating yet..");
        return TRUE;
    }
    LinkedListIterator* metaIterator = LinkedList_getIterator(self->mChildIterators);
    LinkedListIterator* iter = (LinkedListIterator*)(LinkedListIterator_getNext(metaIterator));
    GameObject* obj = (GameObject*)LinkedListIterator_getSame(iter);
    // Check for more child objects:
    if (GameObject_hasChildObjects(obj))
    {
        return TRUE;
    }
    // Check for more siblings, or siblings of parents and grandparents:
    while (!LinkedListIterator_hasMore(iter))
    {
        if (!LinkedListIterator_hasMore(metaIterator))
        {
            return FALSE;
        }
        iter = (LinkedListIterator*)(LinkedListIterator_getNext(metaIterator));
    }
    return TRUE;
}

void GameObjectIterator_delete(GameObjectIterator* self)
{
    LinkedList_delete(self->mRootList);
    LinkedList_delete(self->mChildIterators);
    free(self);
}