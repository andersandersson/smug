#include "interface.h"
#include "common/log.h"
#include "engine.h"

int smugInit()
{
    Engine_init(TRUE, TRUE);
}

void smugTerminate()
{
    Engine_terminate();
}

void smugRun()
{
    if (!Engine_isInitialized())
    {
        ERROR("Engine must be initialized before calling smugRun()");
    }

    Engine_run();
}
