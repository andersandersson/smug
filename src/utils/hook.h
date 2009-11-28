/** @file hook.h
  * @brief Holds a simple struct for callable objects
  */

#ifndef UTILS_HOOK_H
#define UTILS_HOOK_H

#include "linkedlist.h"

/** A struct for a callable object
  * 
  * Create a hook with Hook_new and delete them with 
  * Hook_delete. The 'data' member is usually set by
  * the creater of the Hook and the second parameter
  * to 'function' by the caller of the Hook.
  */ 
typedef struct Hook
{
    /** Parameter 1 to be sent to 'function'. */
    void* data;
   
    /** The function to be called.
      *
      * This function will be called by Hook_call.
      * 'data' will be sent as the first parameter, and
      * the second argument to Hook_call as the second.
      * 
      * @return The value to be returned by Hook_call.
      */
    int (*function)(void*, void*);
} Hook;


/** Create a new hook instance 
  *
  * @return A pointer to the created Hook
  */
Hook* Hook_new();


/** Create a new hook from a function
  *
  * @return A pointer to the created Hook.
  */
Hook* Hook_newFromFunction(void* data, int (*function)(void*, void*));


/** Free the memory used by a Hook
  */
void Hook_delete(void* hook);


/** Call the hook with given parameter
  *
  * This function will call the Hooks member
  * 'function' with 'data' as parameter 1 and
  * 'arg' as parameter 2.
  *
  * @return The value returned by 'function'
  */
int Hook_call(Hook* hook, void* arg);


/** Iterate over a list of Hooks and call them
  *
  * This function will iterate over 'list' and call
  * Hook_call for each item with 'arg' as argument.
  *
  * @return The value of the last called Hook.
  */
int Hook_callAll(LinkedList* list, void* arg); // FIXFIX: Should this really use linkedlist? List already has "for each" functions. Consider new object 'HookList'.


/** Call all Hooks in a list while a condition is true
  *
  * This function will do the same as Hook_callAll except
  * that it will halt and return the first value that
  * is not the same as 'condition'.
  *
  * @return The value of the last called Hook.
  */
int Hook_callAllWhile(LinkedList* list, void* arg, int condition); // FIXFIX: Same as above


/** Call all Hooks in a list while a condition is false
  *
  * This function does the same as Hook_callAllWhile except
  * it will halt on the first value that is the same as condition.
  *
  * @return The value of the last called Hook.
  */
int Hook_callAllWhileNot(LinkedList* list, void* arg, int condition); // FIXFIX: Same as above

#endif // UTILS_HOOK_H
