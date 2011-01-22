#ifndef SMUG_ENGINE_GAMEOBJECTITERATOR_H
#define SMUG_ENGINE_GAMEOBJECTITERATOR_H

#include <common/common.h>

struct GameObject;
struct LinkedList;

typedef struct GameObjectIterator
{
    struct LinkedList* mChildIterators;
    int mTypes;
    BOOL mHasReturnedRoot;
    struct LinkedList* mRootList;
} GameObjectIterator;

GameObjectIterator* GameObject_getIterator(struct GameObject* self, int types);
struct GameObject* GameObjectIterator_getNext(GameObjectIterator* self);
BOOL GameObjectIterator_hasMore(GameObjectIterator* self);
void GameObjectIterator_delete(GameObjectIterator* self);

#endif /* SMUG_ENGINE_GAMEOBJECTITERATOR_H */
