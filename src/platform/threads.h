#ifndef THREAD_H
#define THREAD_H

// Data holder for a thread
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

// Hide the GLFW mutex in our own
typedef struct Mutex
{
   GLFWmutex _mutex;
} Mutex;

// Hide the GLFW condition variable in our own
typedef struct ConditionVariable 
{
   GLFWcond _cond;
} ConditionVariable;


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

#endif // THREAD_H
