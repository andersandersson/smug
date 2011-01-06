#include <smugstd.h>
#include <platform/threads.h>
#include <common/signal.h>

static int _signals = 0;
static Mutex* _signalMutex = NULL;
static BOOL isInitialized = FALSE;

int Signal_init(void)
{
    smug_assert(!isInitialized);
    _signalMutex = Mutex_new();

    if(!Mutex_isInitialized(_signalMutex))
    {
        isInitialized = FALSE;
    }
    else
    {
        isInitialized = TRUE;
    }
    return isInitialized;
}

BOOL Signal_isInitialized(void)
{
    return isInitialized;
}

void Signal_terminate(void)
{
    smug_assert(isInitialized);
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
