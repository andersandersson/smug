#include "signal.h"
#include "platform/threads.h"

static int _signals = 0;
static Mutex* _signalMutex = NULL;
static BOOL isInitialized = FALSE;

int Signal_init()
{
    assert(!isInitialized);
    _signalMutex = Mutex_new();
   
    if(NULL == _signalMutex || NULL == _signalMutex->_mutex)
    {
        isInitialized = FALSE;
    }
    else
    {
        isInitialized = TRUE;
    }
    return isInitialized;
}

BOOL Signal_isInitialized()
{
    return isInitialized;
}

void Signal_terminate()
{
    assert(isInitialized);
    Mutex_delete(_signalMutex);
}

void Signal_send(int signal)
{
    Mutex_lock(_signalMutex);
    _signals = _signals | signal;
    Mutex_unlock(_signalMutex);
}

BOOL Signal_check(int signal)
{
    Mutex_lock(_signalMutex);

    if(signal == (_signals & signal))
        {
            Mutex_unlock(_signalMutex);
            return TRUE;
        }
   
    Mutex_unlock(_signalMutex);

    return FALSE;
}

void Signal_clear(int signal)
{ 
    Mutex_lock(_signalMutex);
    _signals = (_signals & ~signal);
    Mutex_unlock(_signalMutex);
}
