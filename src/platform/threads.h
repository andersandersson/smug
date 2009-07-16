/** @file threads.h
  * @brief Contains functions for multithreading.
  */

#ifndef THREAD_H
#define THREAD_H

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


<<<<<<< HEAD:src/platform/threads.h
/** Create a new thread and set it in a waiting state.
  * 
  * @relatesalso Thread
  */
Thread* Thread_New();

/** Join and destroy the thread.
  * 
  * @relatesalso Thread
  */
void Thread_Delete(Thread* thread);

/** Set the callback function of a sleeping thread and wake it up.
  * 
  * @relatesalso Thread
  */
void Thread_Call(Thread* thread, void (*callback)(void*), void* param);

/** Wait for the callback function to finish.
  * 
  * @relatesalso Thread
  */
void Thread_Join(Thread* thread);

/** Create a new mutex.
  * 
  * @relatesalso Mutex
  */
Mutex* Mutex_New();

/** Destroy a mutex.
  * 
  * @relatesalso Mutex
  */
void Mutex_Delete(Mutex* mutex);

/** Lock a mutex.
  * 
  * @relatesalso Mutex
  */
void Mutex_Lock(Mutex* mutex);

/** Unlock a mutex.
  * 
  * @relatesalso Mutex
  */
void Mutex_Unlock(Mutex* mutex);

/** Create a new condition variable.
  * 
  * @relatesalso ConditionVariable
  */
ConditionVariable* ConditionVariable_New();

/** Destroy a condition variable.
  * 
  * @relatesalso ConditionVariable
  */
void ConditionVariable_Delete(ConditionVariable* cond);

/** Wait for a condition variable.
  * 
  * @relatesalso ConditionVariable
  */
void ConditionVariable_Wait(ConditionVariable* cond, Mutex* mutex, double timeout);

/** Notify one waiting thread.
  * 
  * @relatesalso ConditionVariable
  */
void ConditionVariable_Notify(ConditionVariable* cond);

/** Notify all waiting threads.
  * 
  * @relatesalso ConditionVariable
  */
void ConditionVariable_NotifyAll(ConditionVariable* cond);
=======
// Create a new thread and set it in a waiting state
Thread* Thread_new();

// Join and destroy the thread
void Thread_delete(Thread* thread);

// Set the callback function of a sleeping thread and wake it up
void Thread_call(Thread* thread, void (*callback)(void*), void* param);

// Wait for the callback function to finish
void Thread_join(Thread* thread);


// Create a new mutex
Mutex* Mutex_new();

// Destroy a mutex
void Mutex_delete(Mutex* mutex);

// Lock a mutex
void Mutex_lock(Mutex* mutex);

// Unlock a mutex
void Mutex_unlock(Mutex* mutex);


// Create a new condition variable
ConditionVariable* ConditionVariable_new();

// Destroy a condition variable
void ConditionVariable_delete(ConditionVariable* cond);

// Wait for a condition variable
void ConditionVariable_wait(ConditionVariable* cond, Mutex* mutex, double timeout);

// Notify one waiting thread
void ConditionVariable_notify(ConditionVariable* cond);

// Notify all waiting threads
void ConditionVariable_notifyAll(ConditionVariable* cond);
>>>>>>> e322b9bf1015713e648e1eadb34432aac671860a:src/platform/threads.h

#endif // THREAD_H
