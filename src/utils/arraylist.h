#ifndef SMUG_UTILS_ARRAYLIST_H
#define SMUG_UTILS_ARRAYLIST_H

#include "common/common.h"

typedef struct ArrayList
{
    unsigned int capacity;
    void** list;
} ArrayList;


ArrayList* ArrayList_new();
ArrayList* ArrayList_newFromCapacity(unsigned int capacity);
void ArrayList_delete(void* list);

void ArrayList_set(ArrayList* list, unsigned int index, void* obj);
void* ArrayList_get(ArrayList* list, unsigned int index);
void* ArrayList_remove(ArrayList* list, unsigned int index);
unsigned int ArrayList_getCapacity(ArrayList* list);
void ArrayList_deleteContents(ArrayList* list, void (*deletefunc)(void*));






#endif //SMUG_UTILS_ARRAYLIST_H
