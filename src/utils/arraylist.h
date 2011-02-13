/**
 * @file arraylist.h
 * @brief Defines the ArrayList data structure and related functions.
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_ARRAYLIST_H
#define SMUG_UTILS_ARRAYLIST_H

#include <common/common.h>

/**
 * A static array-based list type. It will expand
 * on demand and can be initiated with a capacity.
 */
typedef struct ArrayList
{
    unsigned int capacity;
    void** list;
} ArrayList;

/**
 * Creates an ArrayList
 * @relates ArrayList
 */
ArrayList* ArrayList_new(void);

/**
 * Creates an ArrayList with a given capacity
 * @param capacity starting capacity
 * @relates ArrayList
 */
ArrayList* ArrayList_newFromCapacity(unsigned int capacity);

/**
 * Deletes an ArrayList
 * @param list pointer to list
 * @relates ArrayList
 */
void ArrayList_delete(void* list);

/**
 * Place an object pointer in the list at an index.
 * @param list pointer to list
 * @param index
 * @param obj pointer to object
 * @relates ArrayList
 */
void ArrayList_set(ArrayList* list, unsigned int index, void* obj);

/**
 * Get the pointer to an object in the list at an index.
 * Returns NULL if the list index was empty or outside list.
 * @param list pointer to list
 * @param index
 * @relates ArrayList
 */
void* ArrayList_get(ArrayList* list, unsigned int index);

/**
 * Remove an object from list, does not free memory.
 * @param list pointer to list
 * @param index
 * @relates ArrayList
 */
void* ArrayList_remove(ArrayList* list, unsigned int index);

/**
 * Get the current capacity of the list, this is not
 * necessarily equal to the size.
 * @param list pointer to list
 * @relates ArrayList
 */
unsigned int ArrayList_getCapacity(ArrayList* list);

/**
 * Delete all objects in list using provided delete-function.
 * @param list pointer to list
 * @param deletefunc pointer to delete function
 * @relates ArrayList
 */
void ArrayList_deleteContents(ArrayList* list, void (*deletefunc)(void*));

#endif //SMUG_UTILS_ARRAYLIST_H

/**@}*/
