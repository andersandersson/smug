#include <common/log.h>
#include <common/common.h>
#include <platform/platform.h>
#include <smugstd.h>
#include <graphics/graphics.h>
#include <input/input.h>
#include <common/signal.h>

static BOOL isInitialized = FALSE;

static void(*gUserWindowResizeCallback)(int, int) = NULL;
static void(*gUserWindowStateChangeCallback)(SMUG_WINDOW_STATE_CHANGE);
static void (*gUserLogicCallback)(void) = NULL;
static BOOL gLogicCallbackEnabled = TRUE;

// Size of window in pixels
static Vector windowSize;

static void setWindowSize(int w, int h)
{
    windowSize = Vector_create2d(w, h);
}

static void windowStateChangeCallback(SMUG_WINDOW_STATE_CHANGE state)
{
    if (gUserWindowStateChangeCallback != NULL)
    {
        gUserWindowStateChangeCallback(state);
    }
    smug_printf("Window state changed: %i", state);
}

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

    setWindowSize(width, height);
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

void Platform_internalHeartbeat(void)
{
    if (gLogicCallbackEnabled && gUserLogicCallback != NULL)
    {
        gUserLogicCallback();
    }
    if(Input_getKey(KEY_ESC) || !Platform_isWindowOpen())
    {
        Signal_send(SIG_EXIT);
    }
}

void Platform_setLogicCallback(void (*callback)(void))
{
    gUserLogicCallback = callback;
}

void Platform_enableLogicCallback(BOOL enable)
{
    gLogicCallbackEnabled = enable;
}

void Platform_setWindowResizeCallback(void(*callback)(int, int))
{
    gUserWindowResizeCallback = callback;
}

void Platform_setWindowStateChangeCallback(void(*callback)(SMUG_WINDOW_STATE_CHANGE))
{
    gUserWindowStateChangeCallback = callback;
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeWindowOpened)
  (JNIEnv* env, jclass clazz)
{
    windowStateChangeCallback(SMUG_OPENED);
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeWindowRestored)
  (JNIEnv* env, jclass clazz)
{
    windowStateChangeCallback(SMUG_RESTORED);
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeWindowActivated)
  (JNIEnv* env, jclass clazz)
{
    windowStateChangeCallback(SMUG_ACTIVATED);
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeWindowDeactivated)
  (JNIEnv* env, jclass clazz)
{
    windowStateChangeCallback(SMUG_DEACTIVATED);
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeWindowMinimized)
  (JNIEnv* env, jclass clazz)
{
    windowStateChangeCallback(SMUG_MINIMIZED);
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeWindowClosed)
  (JNIEnv* env, jclass clazz)
{
    windowStateChangeCallback(SMUG_CLOSED);
}

/** The window resize callback. Called from Android OS (through DroidSmug class).
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeResize)
  (JNIEnv* env, jclass clazz, jint w, jint h)
{
    smug_printf("Window resized: %i x %i", (int)w, (int)h);
    setWindowSize(w, h);
    Graphics_setWindowSize(w, h);
    if (gUserWindowResizeCallback != NULL)
    {
        gUserWindowResizeCallback(w, h);
    }
}

/** If we ever run our own game loop (i.e. "blocking engine") on Android, we must make sure this
 *  doesn't get called, since internalHeartbeat will then get called from our gameloop instead.
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeHeartbeat)
  (JNIEnv* env, jclass clazz)
{
    Platform_internalHeartbeat();
}

TIME Platform_getTime(void)
{
    return (TIME)0;
}

void Platform_sleep(TIME seconds)
{
    // noop for now
}
