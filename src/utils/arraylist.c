#include <smugstd.h>

#include <common/common.h>

#include <utils/arraylist.h>

ArrayList* ArrayList_new(void)
{
    ArrayList* ret = (ArrayList*)malloc(sizeof(ArrayList));
    ret->capacity = 0;
    ret->list = NULL;
    return ret;
}

ArrayList* ArrayList_newFromCapacity(unsigned int capacity)
{
    ArrayList* ret = (ArrayList*)malloc(sizeof(ArrayList));
    ret->capacity = capacity;
    if (capacity > 0)
    {
        ret->list = (void**)malloc(sizeof(void*) * capacity);
        int i;
        for (i = 0; i < capacity; i++)
        {
            ret->list[i] = NULL;
        }
    }
    else
    {
        ret->list = NULL;
    }

    return ret;
}

void ArrayList_delete(void* list)
{
    smug_assert(NULL != list);
    ArrayList* thelist = (ArrayList*)list;
    free(thelist->list);
    free(thelist);
}


static void grow(ArrayList* list, unsigned int newsize)
{
    void** newlist;
    unsigned int oldcap = list->capacity;
    unsigned int newcap = oldcap + 1;

    while (newsize >= newcap)
    {
        newcap*=2;
    }

    newlist = (void**)malloc(sizeof(void*) * newcap);
    int i;
    for (i = 0; i < oldcap; i++)
    {
        newlist[i] = list->list[i];
    }
    for (i = oldcap; i < newcap; i++)
    {
        newlist[i] = NULL;
    }

    free(list->list);
    list->list = newlist;
    list->capacity = newcap;
}



void ArrayList_set(ArrayList* list, unsigned int index, void* obj)
{
    smug_assert(NULL != list);
    if (index < list->capacity)
    {
        list->list[index] = obj;
    }
    else
    {
        grow(list, index);
        list->list[index] = obj;
    }
}

void* ArrayList_get(ArrayList* list, unsigned int index)
{
    smug_assert(NULL != list);
    if (index < list->capacity)
    {
        return list->list[index];
    }
    return NULL;
}

void* ArrayList_remove(ArrayList* list, unsigned int index)
{
    smug_assert(NULL != list);
    void* retval = ArrayList_get(list, index);
    if (index < list->capacity)
    {
        list->list[index] = NULL;
    }
    return retval;
}

unsigned int ArrayList_getCapacity(ArrayList* list)
{
    smug_assert(NULL != list);
    return list->capacity;
}

void ArrayList_deleteContents(ArrayList* list, void (*deletefunc)(void*))
{
    smug_assert(NULL != list);
    int i;
    for (i = 0; i < list->capacity; i++)
    {
        if (list->list[i])
        {
            deletefunc(list->list[i]);
            list->list[i] = NULL;
        }
    }
}


