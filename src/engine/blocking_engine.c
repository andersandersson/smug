#include "common/common.h"
#include "common/signal.h"
#include "platform/platform.h"
#include "graphics/graphics.h"

#include "blocking_engine.h"
#include "engine.h"

void Engine_run(void)
{
    assert(Engine_isInitialized());
    if (!Engine_isInitialized())
        return;

    Signal_clear(SIG_EXIT);

    NOTIFY("Running engine...");
    Log_indent();

    TIME lastFpsCheck = Platform_getTime();
    int fps = 0;

    // Setup timing variables
    TIME delay = 1.0f/30.0f;
    TIME nexttime;
    TIME time;
    nexttime = Platform_getTime();

    while(TRUE != Signal_check(SIG_EXIT))
    {
        time = Platform_getTime();

        if (time >= nexttime)
        {
            nexttime += delay;
            Engine_heartbeat();
        }

        Graphics_render();
        Platform_refreshWindow();

        fps++;

        if (Platform_getTime() - lastFpsCheck >= 1.0)
        {
            //Console_writeLine("Fps: %i", fps);
            //Log_write(LOG_NOTIFICATION, "FPSCK", "engine.c", 116, "%i", fps);
            NOTIFY("Fps: %i", fps);
            fps = 0;
            lastFpsCheck = Platform_getTime();
        }

        // TODO: add if-case: if nexttime-time > smallest_sleep_time
        Platform_sleep(nexttime - time);
    }

    Log_dedent();
    NOTIFY("Engine stopped.");
}
