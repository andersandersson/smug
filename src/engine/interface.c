#include "interface.h"
#include "common/log.h"
#include "engine.h"

int smugInit(void)
{
    Engine_init(TRUE, TRUE);
}

void smugTerminate(void)
{
    Engine_terminate();
}

void smugRun(void)
{
    if (!Engine_isInitialized())
    {
        ERROR("Engine must be initialized before calling smugRun()");
    }

    Engine_run();
}
