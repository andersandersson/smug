/** @file hooks.h
  * @brief Holds a simple struct for callable objects
  */

#ifndef UTILS_HOOK_H
#define UTILS_HOOK_H

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


/** Free the memory used by a Hook
  */
void Hook_delete(Hook* hook);


/** Call the hook with given parameter
  *
  * This function will call the Hooks member
  * 'function' with 'data' as parameter 1 and
  * 'arg' as parameter 2.
  *
  * @return The value returned by 'function'
  */
int Hook_call(Hook* hook, void* arg);

#endif // UTILS_HOOK_H
