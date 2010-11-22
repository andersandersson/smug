/** @file threads.h
  * @brief Contains functions for multithreading.
  */

#ifndef THREAD_H
#define THREAD_H

#include <GL/glfw.h>

/** Data holder for a thread.
  *
  * @sa ::Mutex, ::ConditionVariable
  */
typedef struct Thread
{
    GLFWthread id;

    // "Private" data
    void (*_callback)(void*);
    void* _callbackParam;
    GLFWcond _loopCond;
    GLFWmutex _loopMutex;
    int _alive;
    int _awake;
} Thread;

/** Hide the GLFW mutex in our own.
  *
  * @sa ::Thread, ::ConditionVariable
  */
typedef struct Mutex
{
    GLFWmutex _mutex;
} Mutex;

/** Hide the GLFW condition variable in our own.
  *
  * @sa ::Thread, ::Mutex
  */
typedef struct ConditionVariable
{
    GLFWcond _cond;
} ConditionVariable;


/** Create a new thread and set it in a waiting state.
  *
  * @relatesalso Thread
  */
Thread* Thread_new(char* name);

/** Join and destroy the thread.
  *
  * @relatesalso Thread
  */
void Thread_delete(Thread* thread);

/** Forcibly kill the thread.
  *
  * @relatesalso Thread
  */
void Thread_forceKill(Thread* thread);

/** Set the callback function of a sleeping thread and wake it up.
  *
  * @relatesalso Thread
  */
void Thread_call(Thread* thread, void (*callback)(void*), void* param);

/** Wait for the callback function to finish.
  *
  * @relatesalso Thread
  */
void Thread_join(Thread* thread);

/** Create a new mutex.
  *
  * @relatesalso Mutex
  */
Mutex* Mutex_new(void);

/** Destroy a mutex.
  *
  * @relatesalso Mutex
  */
void Mutex_delete(Mutex* mutex);

/** Lock a mutex.
  *
  * @relatesalso Mutex
  */
void Mutex_lock(Mutex* mutex);

/** Unlock a mutex.
  *
  * @relatesalso Mutex
  */
void Mutex_unlock(Mutex* mutex);

/** Create a new condition variable.
  *
  * @relatesalso ConditionVariable
  */
ConditionVariable* ConditionVariable_new(void);

/** Destroy a condition variable.
  *
  * @relatesalso ConditionVariable
  */
void ConditionVariable_delete(ConditionVariable* cond);

/** Wait for a condition variable.
  *
  * @relatesalso ConditionVariable
  */
void ConditionVariable_wait(ConditionVariable* cond, Mutex* mutex, double timeout);

/** Notify one waiting thread.
  *
  * @relatesalso ConditionVariable
  */
void ConditionVariable_notify(ConditionVariable* cond);

/** Notify all waiting threads.
  *
  * @relatesalso ConditionVariable
  */
void ConditionVariable_notifyAll(ConditionVariable* cond);

#endif // THREAD_H
