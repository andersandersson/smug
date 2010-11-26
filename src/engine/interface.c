#include <smug.h>
#include <common/log.h>
#include "engine.h"

SMUGEXPORT int smugInit(void)
{
    Engine_init(TRUE, TRUE);
}

SMUGEXPORT void smugTerminate(void)
{
    Engine_terminate();
}

/* Commenting this out for now. Want to have possibility of compiling with or without a gameloop internal to smug. */
/* void smugRun(void)
{
    if (!Engine_isInitialized())
    {
        ERROR("Engine must be initialized before calling smugRun()");
    }

    Engine_run();
} */
