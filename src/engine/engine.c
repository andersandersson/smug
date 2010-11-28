#include "engine.h"

#include "stdio.h"

#include "common/signal.h"
#include "common/console.h"
#include "physics/physics.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "common/log.h"
#include "common/common.h"
#include "world.h"
#include "gameobject.h"

Thread* gConsoleThread = NULL;

static BOOL gInitialized = FALSE;
// World* gWorld = NULL;
static LinkedList* gGameObjects = NULL;

// Checks that all subsystems have been initialized.
void _assertSubsystemsInitialized(void)
{
	assert(Log_isInitialized());
    assert(Platform_isInitialized());
    assert(Signal_isInitialized());
    assert(Input_isInitialized());
    assert(Graphics_isInitialized());
    assert(Physics_isInitialized());
}

BOOL _subsystemsInitialized(void)
{
	return Log_isInitialized() &&
           Platform_isInitialized() &&
           Signal_isInitialized() &&
           Input_isInitialized() &&
           Graphics_isInitialized() &&
           Physics_isInitialized();
}

int Engine_init(BOOL verbose, BOOL console)
{
    int logLevel;

    assert(!gInitialized);
    if (!Log_init())
    {
      return 0;
    }

    if (verbose)
    {
        logLevel = Log_getLevel();
        Log_setLevel(LOG_ALL);
    }

    NOTIFY("Initializing engine:");
    Log_indent();

    // TODO: Make screen size value not hardcoded.
    if (!Platform_init(640, 480, FALSE))
        return 0;

    if (!Signal_init())
        return 0;

	if (!Input_init())
		return 0;

    if (!Graphics_init())
        return 0;

    if (!Physics_init())
        return 0;

    if (console)
    {
        NOTIFY("Initializing console thread");
        // Create a new thread for the console
        gConsoleThread = Thread_new("console");

        // Run the console main loop in a new thread
        Thread_call(gConsoleThread, Console_run, NULL);
    }

    // gWorld = World_new();
    gGameObjects = LinkedList_new();

    Log_dedent();
    NOTIFY("Engine Initialized.");
    Log_setLevel(logLevel);

    gInitialized = TRUE;
    _assertSubsystemsInitialized();
//    assert(_subsystemsInitialized);
    return 1;
}

BOOL Engine_isInitialized(void)
{
    return _subsystemsInitialized() && gInitialized;
}

void Engine_terminate(void)
{
    assert(gInitialized);
    NOTIFY("Terminating engine...");
    Log_indent();

    // World_delete(gWorld);
    LinkedList_deleteContents(gGameObjects, GameObject_delete);
    LinkedList_delete(gGameObjects);

    // Do not wait for console thread to end as getc() will
    // block until input is received. Instead, kill the thread.
    if (gConsoleThread != NULL)
    {
        /* This doesn't seem to work, at least not always.
        When console is enabled, the program's process is
        still alive after the main program ends. */
        Thread_forceKill(gConsoleThread);
        gConsoleThread = NULL;
    }

    Physics_terminate();

    Graphics_terminate();

	Input_terminate();

    Signal_terminate();

    Platform_terminate();

    Log_dedent();
    NOTIFY("Engine terminated.\n");

    Log_terminate();

    gInitialized = FALSE;
}

void Engine_addObject(GameObject* newObj)
{
    assert(gInitialized);
    LinkedList_addLast(gGameObjects, newObj);
    // Add all drawables in the object to the graphics engine.
    Node* node;
    for (node = newObj->drawables->first; node != NULL; node = node->next)
    {
        Graphics_addDrawable((Drawable*)node->item);
    }
}

void Engine_removeObject(GameObject* obj)
{
    assert(gInitialized);
    LinkedList_removeItem(gGameObjects, obj);
    Node* node;
    for (node = obj->drawables->first; node != NULL; node = node->next)
    {
        Graphics_removeDrawable((Drawable*)node->item);
    }
}
