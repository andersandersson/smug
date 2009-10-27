#include "engine.h"

#include "stdio.h"

#include "common/signal.h"
#include "common/console.h"
#include "platform/platform.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "input/input.h"
#include "common/log.h"
#include "world.h"
#include "gameobject.h"

Thread* gConsoleThread = NULL;

static BOOL gInitialized = FALSE; 

World* gWorld = NULL;

// Checks that all subsystems have been initialized.
void _assertSubsystemsInitialized()
{
	assert(Log_isInitialized());
    assert(Platform_isInitialized());
    assert(Signal_isInitialized());
    assert(Input_isInitialized());
    assert(Graphics_isInitialized());
    assert(Physics_isInitialized());
}

BOOL _subsystemsInitialized()
{
	return Log_isInitialized() &&
           Platform_isInitialized() &&
           Signal_isInitialized() &&
           Input_isInitialized() &&
           Graphics_isInitialized() &&
           Physics_isInitialized();
}

int Engine_init(BOOL verbose)
{
    int logLevel;
    
    assert(!gInitialized);
    Log_init();
    if (verbose)
    {
        logLevel = Log_getLevel();
        Log_setLevel(LOG_ALL);
    }

    NOTIFY("Initializing engine:");
    Log_indent();
    

    if (!Platform_init(640, 480, FALSE))
        return 0;
    
    if (!Signal_init())
        return 0;

	if (!Input_init())
		return 0;
		
    if (!Graphics_init(640, 480))
        return 0;
        
    if (!Physics_init())
        return 0;

    NOTIFY("Initializing console thread");
    // Create a new thread for the console
    gConsoleThread = Thread_new();

    // Run the console main loop in a new thread
    Thread_call(gConsoleThread, Console_run, NULL);    
    
    gWorld = World_new();

    Log_dedent();
    NOTIFY("Engine Initialized.");
    Log_setLevel(logLevel);
    
    gInitialized = TRUE;
    _assertSubsystemsInitialized();
//    assert(_subsystemsInitialized);
    return 1;
}

BOOL Engine_isInitialized()
{
    return _subsystemsInitialized() && gInitialized;
}

void Engine_terminate()
{
    assert(gInitialized);
    NOTIFY("Terminating engine");
    Log_indent();
    
    World_delete(gWorld);
    
    // Do not wait for console thread to end as getc() will
    // block until input is received. Instead, kill the thread.
    Thread_forceKill(gConsoleThread);
    
    Physics_terminate();
    
    Graphics_terminate();
    
	Input_terminate();
	
    Signal_terminate();

    Platform_terminate();
    
    Log_dedent();
    NOTIFY("Engine terminated,\n");
    
    Log_terminate();
    
    gInitialized = FALSE;
}

void Engine_run()
{
    assert(gInitialized);
    if (!gInitialized)
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
            nexttime+=delay;
        
            if(Input_getKey(KEY_ESC) || !Platform_isWindowOpen())
            {
                Signal_send(SIG_EXIT);
            }
            
            // Render the world.
            //World_render(gWorld);
            Graphics_render();
            Platform_refreshWindow();
            
            fps++;
        }
        
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

void Engine_addObject(GameObject* newObj)
{
    World_addObject(gWorld, newObj);
}
