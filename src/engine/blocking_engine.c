#include <smugstd.h>
#include <common/common.h>
#include <common/signal.h>
#include <platform/platform.h>
#include <graphics/graphics.h>

#include <engine/engine.h>
#include <engine/blocking_engine.h>

static TIME delay = 0.0;
static BOOL fpsSet = FALSE;
static float gFps = 20.0f;

static float gInterpolationFactor; /**< Fraction of a whole heartbeat time that has passed since the last heartbeat. */
static TIME gLastHeartbeat;

void Engine_run(void)
{
    smug_assert(Engine_isInitialized());
    if (!Engine_isInitialized())
        return;

    Signal_clear(SIG_EXIT);

    NOTIFY("Running engine...");
    Log_indent();

    TIME lastFpsCheck = Platform_getTime();
    int fps = 0;

    // Setup timing variables
    if (!fpsSet)
    {
        Engine_setLogicFps(30.0f);
    }
    TIME nexttime;
    TIME time;
    nexttime = Platform_getTime();

    while(TRUE != Signal_check(SIG_EXIT))
    {
        time = Platform_getTime();

        if (time >= nexttime)
        {
            nexttime += delay;
            Platform_stepDiscreteTime();
            gLastHeartbeat = Platform_getDiscreteTime();
            Platform_internalHeartbeat();
            Engine_commitPositionChanges();
        }

        Platform_stepDiscreteTime();
        gInterpolationFactor = (Platform_getDiscreteTime() - gLastHeartbeat) / (1.0 / gFps);
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

float Engine_getInterpolationFactor(void)
{
    return gInterpolationFactor;
}

void Engine_setLogicFps(float fps)
{
    gFps = fps;
    delay = 1.0 / fps;
    fpsSet = TRUE;
}
