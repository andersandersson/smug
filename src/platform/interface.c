#include <platform/platform.h>
#include <platform/interface.h>

SMUGEXPORT void smugSetLogicCallback(void (*callback)(void))
{
    Platform_setLogicCallback(callback);
}

SMUGEXPORT double smugGetTime(void)
{
    return Platform_getTime();
}

SMUGEXPORT void smugSetLogicFps(float fps)
{
    Platform_setLogicFps(fps);
}

SMUGEXPORT void smugSetKillCallback(void(*callback)(void))
{
    Platform_setKillCallback(callback);
}
