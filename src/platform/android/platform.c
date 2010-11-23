#include <common/log.h>
#include <platform/platform.h>

static BOOL isInitialized = FALSE;

// Size of window in pixels
static Vector windowSize;

int Platform_init(int width, int height, BOOL fullscreen)
{
    assert(!isInitialized);
    NOTIFY("Initializing platform layer");
    // if (!glfwInit())
    // {
        // return 0;
    // }

    // glfwDisable(GLFW_AUTO_POLL_EVENTS);
    // glfwEnable(GLFW_STICKY_KEYS);
    // glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);

    windowSize = Vector_create2d(width, height);
    // if (!glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 0, fullscreen? GLFW_FULLSCREEN : GLFW_WINDOW))
    // {
        // ERROR("Could not open window.");
        // return 0;
    // }

    Platform_initInput();
    isInitialized = TRUE;
    return 1;
}

BOOL Platform_isInitialized(void)
{
    return isInitialized;
}

void Platform_terminate(void)
{
    assert(isInitialized);
    NOTIFY("Terminating platform layer");
    // glfwCloseWindow();
    // glfwTerminate();
    isInitialized = FALSE;
}

BOOL Platform_isWindowOpen(void)
{
    return TRUE;
}

void Platform_refreshWindow(void)
{
    // noop
}

Vector Platform_getWindowSize(void)
{
    return windowSize;
}

TIME Platform_getTime(void)
{
    return (TIME)0;
}

void Platform_sleep(TIME seconds)
{
    // noop for now
}
