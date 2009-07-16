#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

#include "threads.h"

// Internal functions that should not be used
// outside threads.c

// The function that will wait for callbacks to run
static void GLFWCALL Thread_Loop(void* arg);

// Signal the _loopCond variable
static void Thread_Signal(Thread* thread);

// Lock the _loopMutex
static void Thread_Lock(Thread* thread);

// Release the _loopMutex
static void Thread_Release(Thread* thread);



Thread* Thread_New(char* name)
{
   Thread* thread = malloc(sizeof(Thread));
   
   thread->_alive = 0;
   thread->_awake = 0;
   thread->_callbackParam = NULL;
   thread->_callback = NULL;
   thread->_loopCond = glfwCreateCond();
   thread->_loopMutex = glfwCreateMutex();

   // Create a new thread and have it run the Thread_Loop
   thread->id = glfwCreateThread(Thread_Loop, (void*) thread);
   
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


void Thread_Signal(Thread* thread)
{
   glfwSignalCond(thread->_loopCond);
}


void Thread_Lock(Thread* thread)
{
   glfwLockMutex(thread->_loopMutex);
}


void Thread_Release(Thread* thread)
{
   glfwUnlockMutex(thread->_loopMutex);
}


void Thread_Join(Thread* thread)
{
   Thread_Lock(thread);
  
   while(1 == thread->_awake) {
      Thread_Signal(thread);
      glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
   }
  
   Thread_Release(thread);
}


void Thread_Call(Thread* thread, void (*callback)(void*), void* param)
{
   Thread_Lock(thread);
   thread->_callback = callback;
   thread->_callbackParam = param;
   thread->_awake = 1;
   Thread_Release(thread);
   Thread_Signal(thread);
}


void Thread_Delete(Thread* thread)
{
   Thread_Lock(thread);

   thread->_alive = 0;
   thread->_awake = 1;

   Thread_Signal(thread);

   glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
   glfwWaitThread(thread->id, GLFW_INFINITY);

   Thread_Release(thread);

   glfwDestroyMutex(thread->_loopMutex);
   glfwDestroyCond(thread->_loopCond);
   glfwDestroyThread(thread->id);

   free(thread);
}

void GLFWCALL Thread_Loop(void* arg)
{
   Thread* thread = (Thread*) arg;

   glfwLockMutex(thread->_loopMutex);

   thread->_alive = 1;
   thread->_awake = 0;

   glfwSignalCond(thread->_loopCond);

   while(1 == thread->_alive) {    
      while(0 == thread->_awake) {
         Thread_Signal(thread);
         glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
      }

      if(NULL != thread->_callback && 1 == thread->_alive) {
         thread->_callback(thread->_callbackParam);
      }

      thread->_awake = 0;    
   }

   Thread_Signal(thread);
   glfwUnlockMutex(thread->_loopMutex);
}



Mutex* Mutex_New()
{
   Mutex* mutex = malloc(sizeof(Mutex));

   mutex->_mutex = glfwCreateMutex();
}


void Mutex_Delete(Mutex* mutex)
{
   glfwDestroyMutex(mutex->_mutex);

   free(mutex);
}


void Mutex_Lock(Mutex* mutex)
{
   glfwLockMutex(mutex->_mutex);
}


void Mutex_Unlock(Mutex* mutex)
{
   glfwUnlockMutex(mutex->_mutex);
}




ConditionVariable* ConditionVariable_New()
{
   ConditionVariable* cond = malloc(sizeof(ConditionVariable));

   cond->_cond = glfwCreateCond();

   return cond;
}


void ConditionVariable_Delete(ConditionVariable* cond)
{
   glfwDestroyCond(cond->_cond);

   free(cond);
}


void ConditionVariable_Wait(ConditionVariable* cond, Mutex* mutex, double timeout)
{
   glfwWaitCond(cond->_cond, mutex->_mutex, timeout);
}


void ConditionVariable_Notify(ConditionVariable* cond)
{
   glfwSignalCond(cond->_cond);
}


void ConditionVariable_NotifyAll(ConditionVariable* cond)
{
   glfwBroadcastCond(cond->_cond);
}
