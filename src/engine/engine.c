#include "engine.h"

#include "stdio.h"

#include "GL/glfw.h"
#include "common/signal.h"
#include "common/console.h"
#include "platform/platform.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "common/log.h"
#include "world.h"
#include "gameobject.h"

Thread* gConsoleThread = NULL;

static BOOL gInitialized = FALSE; 

World* gWorld = NULL;

int Engine_init()
{
    NOTIFY("Initializing engine:");
    Log_indent();
    
    NOTIFY("Initializing platform layer");
    if (!glfwInit())
        return 0;

    if (!Graphics_init(640, 480, FALSE))
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
    
    gInitialized = TRUE;
    return 1;
}

void Engine_terminate()
{
    NOTIFY("Terminating engine");
    Log_indent();
    
    Physics_terminate();
    
    Graphics_terminate();
    
    World_delete(gWorld);
    
    NOTIFY("Terminating platform layer");
    glfwTerminate();
    
    Log_dedent();
    NOTIFY("Engine terminated,\n");
    
    // Do not wait for console thread to end as getc() will
    // block until input is received. Instead, kill the thread.
    glfwDestroyThread(gConsoleThread->id);    
    
    gInitialized = FALSE;
}

void Engine_run()
{
    if (!gInitialized)
        return;
        
    NOTIFY("Running engine...");
    Log_indent();
    
    double lastFpsCheck = glfwGetTime();
    int fps = 0;
    
    // Setup timing variables
    Time delay = 1.0f/30.0f;
    Time nexttime;
    Time time;
    nexttime = glfwGetTime();
    
    while(TRUE != Signal_checkAndClear(SIG_EXIT))
    {
        time = glfwGetTime();
        
        if (time >= nexttime)
        {
            nexttime+=delay;
        
            if(glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED ))
            {
                Signal_send(SIG_EXIT);
            }
            if(glfwGetKey( GLFW_KEY_F1 ))
            {
                Script_terminate();
                Signal_send(SIG_EXIT);
                Signal_send(SIG_TERM);
            }
            
            // Render the world.
            World_render(gWorld);
            Graphics_refresh();
            
            fps++;
        }
        
        if (glfwGetTime() - lastFpsCheck >= 1.0)
        {
            Console_writeLine("Fps: %i", fps);
            fps = 0;
            lastFpsCheck = glfwGetTime();
        }
        
        // TODO: add if-case: if nexttime-time > smallest_sleep_time
        glfwSleep(nexttime - time);
    }
    
    Log_dedent();
    NOTIFY("Engine stopped.\n");
}

void Engine_addObject(GameObject* newObj)
{
    World_addObject(gWorld, newObj);
}
