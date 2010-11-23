#include <pthread.h>
#include <platform/threads.h>

struct _Thread
{
    pthread_t id;

    // "Private" data
    void (*_callback)(void*);
    void* _callbackParam;
    pthread_cond_t _loopCond;
    pthread_mutex_t _loopMutex;
    int _alive;
    int _awake;
};

struct _Mutex
{
    pthread_mutex_t _mutex;
};

struct _ConditionVariable
{
    pthread_cond_t _cond;
};

// Internal functions that should not be used
// outside threads.c

// The function that will wait for callbacks to run
static void* Thread_loop(void* arg);

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

    // thread->_loopCond = glfwCreateCond();
    pthread_cond_init(&thread->_loopCond, NULL);

    // thread->_loopMutex = glfwCreateMutex();
    pthread_mutex_init(&thread->_loopMutex, NULL);

    // Create a new thread and have it run the Thread_Loop
    // Make it explicitly joinable.
    // thread->id = glfwCreateThread(Thread_loop, (void*) thread);
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread->id, &attributes, Thread_loop, (void*) thread);
    pthread_attr_destroy(&attributes);

    // Acquire the loop lock
    // glfwLockMutex(thread->_loopMutex);
    Thread_lock(thread);

    // Wait for the thread to say that it is alive (to prevent
    // that someone kills it before it has became alive).
    while(0 == thread->_alive) {
        // glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
        pthread_cond_wait(&thread->_loopCond, &thread->_loopMutex);
    }

    // Release the loop lock and return
    // glfwUnlockMutex(thread->_loopMutex);
    Thread_release(thread);

    return thread;
}

void Thread_signal(Thread* thread)
{
    pthread_cond_signal(&thread->_loopCond);
   // glfwSignalCond(thread->_loopCond);
}

void Thread_lock(Thread* thread)
{
   // glfwLockMutex(thread->_loopMutex);
    pthread_mutex_lock(&thread->_loopMutex);
}

void Thread_release(Thread* thread)
{
   // glfwUnlockMutex(thread->_loopMutex);
    pthread_mutex_unlock(&thread->_loopMutex);
}

void Thread_join(Thread* thread)
{
    Thread_lock(thread);

    while(1 == thread->_awake) {
        Thread_signal(thread);
        // glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
        pthread_cond_wait(&thread->_loopCond, &thread->_loopMutex);
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

    // glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
    pthread_cond_wait(&thread->_loopCond, &thread->_loopMutex);
    // glfwWaitThread(thread->id, GLFW_INFINITY);
    void* return_value;
    pthread_join(thread->id, &return_value);

    Thread_release(thread);

    // glfwDestroyMutex(thread->_loopMutex);
    pthread_mutex_destroy(&thread->_loopMutex);
    // glfwDestroyCond(thread->_loopCond);
    pthread_cond_destroy(&thread->_loopCond);

    free(thread);
}

void Thread_forceKill(Thread* thread)
{
    // glfwDestroyThread(thread->id);
    // pthread_kill(thread->id, );
    // if (pthread_cancel(thread->id) == 0)
    // {
        // free(thread);
    // }
    Thread_delete(thread);
}

void* Thread_loop(void* arg)
{
    Thread* thread = (Thread*) arg;

    // glfwLockMutex(thread->_loopMutex);
    Thread_lock(thread);

    thread->_alive = 1;
    thread->_awake = 0;

    // glfwSignalCond(thread->_loopCond);
    Thread_signal(thread);

    while(1 == thread->_alive)
    {
        while(0 == thread->_awake)
        {
            Thread_signal(thread);
            // glfwWaitCond(thread->_loopCond, thread->_loopMutex, GLFW_INFINITY);
            pthread_cond_wait(&thread->_loopCond, &thread->_loopMutex);
        }
        if(NULL != thread->_callback && 1 == thread->_alive)
        {
            thread->_callback(thread->_callbackParam);
        }
        thread->_awake = 0;
    }

    Thread_signal(thread);
    // glfwUnlockMutex(thread->_loopMutex);
    void* ret = thread->_callbackParam;
    Thread_release(thread);
    return ret;
}

Mutex* Mutex_new(void)
{
    Mutex* mutex = (Mutex*)malloc(sizeof(Mutex));
    if (pthread_mutex_init(&mutex->_mutex, NULL) == 0)
    {
        return mutex;
    }
    else
    {
        free(mutex);
        return NULL;
    }
}

void Mutex_delete(Mutex* mutex)
{
    pthread_mutex_destroy(&mutex->_mutex);
    free(mutex);
}

BOOL Mutex_isInitialized(Mutex* self)
{
    return self != NULL;
}

void Mutex_lock(Mutex* mutex)
{
    pthread_mutex_lock(&mutex->_mutex);
}

void Mutex_unlock(Mutex* mutex)
{
    pthread_mutex_unlock(&mutex->_mutex);
}

ConditionVariable* ConditionVariable_new(void)
{
    ConditionVariable* cond = malloc(sizeof(ConditionVariable));
    pthread_cond_init(&cond->_cond, NULL);
    return cond;
}

void ConditionVariable_delete(ConditionVariable* cond)
{
    pthread_cond_destroy(&cond->_cond);
    free(cond);
}

void ConditionVariable_wait(ConditionVariable* cond, Mutex* mutex, double timeout)
{
    pthread_cond_wait(&cond->_cond, &mutex->_mutex);
    // TODO: Use pthread_cond_timedwait instead.
}

void ConditionVariable_notify(ConditionVariable* cond)
{
    pthread_cond_signal(&cond->_cond);
}

void ConditionVariable_notifyAll(ConditionVariable* cond)
{
    pthread_cond_broadcast(&cond->_cond);
}
