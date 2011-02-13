/**
 * @file threads.h
 * @brief Contains functions for multithreading.
 * @if doc_files
 * @ingroup smug_platform
 * @endif
 */

/**
 * @addtogroup smug_platform
 * @{
 */

#ifndef SMUG_PLATFORM_THREADS_H
#define SMUG_PLATFORM_THREADS_H

#include <common/common.h>

struct _Thread;
typedef struct _Thread Thread;

struct _Mutex;
typedef struct _Mutex Mutex;

struct _ConditionVariable;
typedef struct _ConditionVariable ConditionVariable;

#ifdef _DOXYGEN
/**
 * Data holder for a thread.
 * @sa ::Mutex, ::ConditionVariable
 */
struct _Thread {};

/**
 * Hide the GLFW mutex in our own.
 * @sa ::Thread, ::ConditionVariable
 */
struct _Mutex {};

/**
 * Hide the GLFW condition variable in our own.
 * @sa ::Thread, ::Mutex
 */
struct _ConditionVariable {};
#endif

/**
 * Create a new thread and set it in a waiting state.
 * @relates _Thread
 */
Thread* Thread_new(char* name);

/**
 * Join and destroy the thread.
 * @relates _Thread
 */
void Thread_delete(Thread* thread);

/**
 * Forcibly kill the thread.
 * @relates _Thread
 */
void Thread_forceKill(Thread* thread);

/**
 * Set the callback function of a sleeping thread and wake it up.
 * @relates _Thread
 */
void Thread_call(Thread* thread, void (*callback)(void*), void* param);

/**
 * Wait for the callback function to finish.
 * @relates _Thread
 */
void Thread_join(Thread* thread);

/**
 * Create a new mutex.
 * @relates _Mutex
 */
Mutex* Mutex_new(void);

/**
 * Destroy a mutex.
 * @relates _Mutex
 */
void Mutex_delete(Mutex* mutex);

BOOL Mutex_isInitialized(Mutex* self);

/**
 * Lock a mutex.
 * @relates _Mutex
 */
void Mutex_lock(Mutex* mutex);

/**
 * Unlock a mutex.
 * @relates _Mutex
 */
void Mutex_unlock(Mutex* mutex);

/**
 * Create a new condition variable.
 * @relates _ConditionVariable
 */
ConditionVariable* ConditionVariable_new(void);

/**
 * Destroy a condition variable.
 * @relates _ConditionVariable
 */
void ConditionVariable_delete(ConditionVariable* cond);

/**
 * Wait for a condition variable.
 * @relates _ConditionVariable
 */
void ConditionVariable_wait(ConditionVariable* cond, Mutex* mutex, double timeout);

/**
 * Notify one waiting thread.
 * @relates _ConditionVariable
 */
void ConditionVariable_notify(ConditionVariable* cond);

/**
 * Notify all waiting threads.
 * @relates _ConditionVariable
 */
void ConditionVariable_notifyAll(ConditionVariable* cond);

#endif // SMUG_PLATFORM_THREADS_H

/**@}*/
