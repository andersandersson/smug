#include <platform/platform.h>
#include "interface.h"

void smugSetLogicCallback(void (*callback)(void))
{
    Platform_setLogicCallback(callback);
}

double smugGetTime(void)
{
    Platform_getTime();
}
