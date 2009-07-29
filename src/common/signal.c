#include "signal.h"
#include "platform/threads.h"

static int _signals = 0;
static Mutex* _signalMutex = NULL;

int Signal_init()
{
    _signalMutex = Mutex_new();
   
    if(NULL == _signalMutex || NULL == _signalMutex->_mutex)
        {            
            return FALSE;
        }
    else
        {
            return TRUE;
        }
}

void Signal_terminate()
{
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
