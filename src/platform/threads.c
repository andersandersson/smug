#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

#include "threads.h"

// Internal functions that should not be used
// outside threads.c

// The function that will wait for callbacks to run
static void GLFWCALL Thread_loop(void* arg);

// Signal the _loopCond variable
static void Thread_signal(Thread* thread);

// Lock the _loopMutex
static void Thread_lock(Thread* thread);

// Release the _loopMutex
static void Thread_release(Thread* thread);



Thread* Thread_new(char* name)
{
   Thread* thread = malloc(sizeof(Thread));
   
   thread->_alive = 0;
   thread->_awake = 0;
   thread->_callbackParam = NULL;
   thread->_callback = NULL;
   thread->_loopCond = glfwCreateCond();
   thread->_loopMutex = glfwCreateMutex();

   // Create a new thread and have it run the Thread_Loop
   thread->id = glfwCreateThread(Thread_loop, (void*) thread);
   
   // Acquire the loop lock
   glfwLockMutex(thread->_loopMutex);

   // Wait for the thread to say that it is alive (to prevent
   // that someone kills it before it has became alive).
   while(0 == thread->_alive) {
      glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
   }

   // Release the loop lock and return
   glfwUnlockMutex(thread->_loopMutex);

   return thread;
}


void Thread_signal(Thread* thread)
{
   glfwSignalCond(thread->_loopCond);
}


void Thread_lock(Thread* thread)
{
   glfwLockMutex(thread->_loopMutex);
}


void Thread_release(Thread* thread)
{
   glfwUnlockMutex(thread->_loopMutex);
}


void Thread_join(Thread* thread)
{
   Thread_lock(thread);
  
   while(1 == thread->_awake) {
      Thread_signal(thread);
      glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
   }
  
   Thread_release(thread);
}


void Thread_call(Thread* thread, void (*callback)(void*), void* param)
{
   Thread_lock(thread);
   thread->_callback = callback;
   thread->_callbackParam = param;
   thread->_awake = 1;
   Thread_release(thread);
   Thread_signal(thread);
}


void Thread_delete(Thread* thread)
{
   Thread_lock(thread);

   thread->_alive = 0;
   thread->_awake = 1;

   Thread_signal(thread);

   glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
   glfwWaitThread(thread->id, GLFW_INFINITY);

   Thread_release(thread);

   glfwDestroyMutex(thread->_loopMutex);
   glfwDestroyCond(thread->_loopCond);
   glfwDestroyThread(thread->id);

   free(thread);
}

void Thread_forceKill(Thread* thread)
{
   glfwDestroyThread(thread->id);
   free(thread);
}

void GLFWCALL Thread_loop(void* arg)
{
   Thread* thread = (Thread*) arg;

   glfwLockMutex(thread->_loopMutex);

   thread->_alive = 1;
   thread->_awake = 0;

   glfwSignalCond(thread->_loopCond);

   while(1 == thread->_alive) {    
      while(0 == thread->_awake) {
         Thread_signal(thread);
         glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
      }

      if(NULL != thread->_callback && 1 == thread->_alive) {
         thread->_callback(thread->_callbackParam);
      }

      thread->_awake = 0;    
   }

   Thread_signal(thread);
   glfwUnlockMutex(thread->_loopMutex);
}



Mutex* Mutex_new(void)
{
   Mutex* mutex = malloc(sizeof(Mutex));

   mutex->_mutex = glfwCreateMutex();

   return mutex;
}


void Mutex_delete(Mutex* mutex)
{
   glfwDestroyMutex(mutex->_mutex);

   free(mutex);
}


void Mutex_lock(Mutex* mutex)
{
   glfwLockMutex(mutex->_mutex);
}


void Mutex_unlock(Mutex* mutex)
{
   glfwUnlockMutex(mutex->_mutex);
}




ConditionVariable* ConditionVariable_new(void)
{
   ConditionVariable* cond = malloc(sizeof(ConditionVariable));

   cond->_cond = glfwCreateCond();

   return cond;
}


void ConditionVariable_delete(ConditionVariable* cond)
{
   glfwDestroyCond(cond->_cond);

   free(cond);
}


void ConditionVariable_wait(ConditionVariable* cond, Mutex* mutex, double timeout)
{
   glfwWaitCond(cond->_cond, mutex->_mutex, timeout);
}


void ConditionVariable_notify(ConditionVariable* cond)
{
   glfwSignalCond(cond->_cond);
}


void ConditionVariable_notifyAll(ConditionVariable* cond)
{
   glfwBroadcastCond(cond->_cond);
}
