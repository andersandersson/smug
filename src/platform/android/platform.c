#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <common/signal.h>
#include <utils/vector.h>
#include <graphics/graphics.h>
#include <input/input.h>
#include <engine/engine.h>

#include <platform/platform.h>

static BOOL isInitialized = FALSE;

static void(*gUserWindowResizeCallback)(int, int) = NULL;
static void(*gUserWindowStateChangeCallback)(SMUG_WINDOW_STATE_CHANGE) = NULL;
static void(*gUserTouchEventCallback)(int, int, int) = NULL;
static void (*gUserLogicCallback)(void) = NULL;
static BOOL gLogicCallbackEnabled = TRUE;

static jclass gHeartbeatClass = NULL;
static jmethodID gHeartbeat_changeFps = NULL;
static JavaVM* gJavaVm = NULL;

// Size of window in pixels
static Vector windowSize;

static TIME gDiscreteTime;
static TIME gLastHeartbeat;
static float gInterpolationFactor; /**< Fraction of a whole heartbeat time that has passed since the last heartbeat. */
static float gFps = 20.0f;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    gJavaVm = vm;
    return (jint)JNI_VERSION_1_2;
}

static JNIEnv* getJniEnvironment()
{
    smug_assert(gJavaVm != NULL);
    JNIEnv* env;
    if(0 != (*gJavaVm)->AttachCurrentThread(gJavaVm, &env, NULL))
    {
        // Error
    }
    return env;
}

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
    smug_assert(!isInitialized);
    NOTIFY("Initializing platform layer");

    setWindowSize(width, height);

    JNIEnv* env = getJniEnvironment();
    gHeartbeatClass = JCALL1(env, FindClass, "se/lolektivet/droidsmug/Heartbeat");
    gHeartbeat_changeFps = JCALL3(env, GetStaticMethodID, gHeartbeatClass, "changeFps", "(F)V");

    Platform_stepDiscreteTime();

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
    smug_assert(isInitialized);
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

void Platform_setLogicFps(float fps)
{
    gFps = fps;
    JNIEnv* env = getJniEnvironment();
    JCALL3(env, CallStaticVoidMethod, gHeartbeatClass, gHeartbeat_changeFps, (jfloat)fps);
}

void Platform_setTouchEventCallback(void(*callback)(int, int, int))
{
    gUserTouchEventCallback = callback;
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
    Platform_enableLogicCallback(TRUE);
    windowStateChangeCallback(SMUG_ACTIVATED);
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeWindowDeactivated)
  (JNIEnv* env, jclass clazz)
{
    Platform_enableLogicCallback(FALSE);
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
    Platform_stepDiscreteTime();
    gLastHeartbeat = Platform_getDiscreteTime();
    Platform_internalHeartbeat();
    Engine_commitPositionChanges();
}

float Platform_getInterpolationFactor()
{
    return gInterpolationFactor;
}

TIME Platform_getTime(void)
{
    return ((double)clock()) / ((double)CLOCKS_PER_SEC);
}

TIME Platform_getDiscreteTime(void)
{
    return gDiscreteTime;
}

void Platform_stepDiscreteTime(void)
{
    gDiscreteTime = Platform_getTime();
}

void Platform_sleep(TIME seconds)
{
    // noop for now
}

/**************************************/
/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeInit
 * Signature: (F)V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeInit)
  (JNIEnv* env, jclass clazz, jfloat fps)
{
    Engine_init(TRUE, TRUE);
    // smugInit();
    smug_printf("Hello smug world!");
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeRender
 * Signature: ()V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeRender)
  (JNIEnv* env, jclass clazz)
{
    Platform_stepDiscreteTime();
    gInterpolationFactor = (gDiscreteTime - gLastHeartbeat) / (1.0 / gFps);
    Graphics_render();
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeDone
 * Signature: ()V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeDone)
  (JNIEnv* env, jclass clazz)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeTouchDown
 * Signature: ()Z
 */
SMUGEXPORT jboolean JNICALL JAVA_IMPLEMENTATION(nativeTouchDown)
  (JNIEnv* env, jclass clazz)
{
    return JNI_FALSE;
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeTouchUp
 * Signature: ()Z
 */
SMUGEXPORT jboolean JNICALL JAVA_IMPLEMENTATION(nativeTouchUp)
  (JNIEnv* env, jclass clazz)
{
    return JNI_FALSE;
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeKeyDown
 * Signature: (I)Z
 */
SMUGEXPORT jboolean JNICALL JAVA_IMPLEMENTATION(nativeKeyDown)
  (JNIEnv* env, jclass clazz, jint keyCode)
{
    if (keyCode == 4)
    {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeKeyUp
 * Signature: (I)Z
 */
SMUGEXPORT jboolean JNICALL JAVA_IMPLEMENTATION(nativeKeyUp)
  (JNIEnv* env, jclass clazz, jint keyCode)
{
    if (keyCode == 4)
    {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeOrientationChange
 * Signature: (I)V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeOrientationChange)
  (JNIEnv* env, jclass clazz, jint orientation)
{
}

