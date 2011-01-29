#include <smugstd.h>

#include <common/common.h>
#include <common/log.h>
#include <common/signal.h>
#include <utils/vector.h>
#include <graphics/graphics.h>
#include <input/input.h>
#include <engine/engine.h>
#include <platform/file.h>

#include <platform/platform.h>

static BOOL isInitialized = FALSE;

static void(*gUserWindowResizeCallback)(int, int) = NULL;
static void(*gUserWindowStateChangeCallback)(SMUG_WINDOW_STATE_CHANGE) = NULL;
static void(*gUserTouchEventCallback)(int, int, int) = NULL;
static void (*gUserLogicCallback)(void) = NULL;
static void (*gUserKillCallback)(void) = NULL;
static BOOL gLogicCallbackEnabled = TRUE;

static jclass gNativeCallbacksClass = NULL;
static jmethodID gNativeCallbacks_openFile = NULL;
static jmethodID gNativeCallbacks_changeFps = NULL;
static JavaVM* gJavaVm = NULL;

// Size of window in pixels
static Vector windowSize;

static TIME gDiscreteTime;
static TIME gLastHeartbeat;
static float gInterpolationFactor; /**< Fraction of a whole heartbeat time that has passed since the last heartbeat. */
static float gFps = 20.0f;

static int gFpsCounter = 0;
static TIME gLastFpsCheck = 0.0;
static TIME gFpsCheckInterval = 2.0;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    gJavaVm = vm;
    return (jint)JNI_VERSION_1_2;
}

static JNIEnv* getJniEnvironment(void)
{
    smug_assert(gJavaVm != NULL);
    JNIEnv* env = NULL;
    if(0 != (*gJavaVm)->AttachCurrentThread(gJavaVm, &env, NULL))
    {
        ERROR("Could not attach to the Java thread.");
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

/* Implementation of File class */

struct _File
{
    unsigned char* mData;
    long int mLength;
    long int mPosition;
};

static File* gOpeningFile = NULL;

static File* File_new(unsigned char* data, long int length)
{
    File* newFile = (File*)malloc(sizeof(File));
    newFile->mLength = length;
    newFile->mData = data;
    newFile->mPosition = 0;
    smug_assert(data != NULL);
    // return NULL;
    return newFile;
}

File* File_fopen(const char* filename, const char* mode)
{
    smug_assert(gOpeningFile == NULL);
    JNIEnv* env = getJniEnvironment();
    jstring str = JCALL1(env, NewStringUTF, filename);
    JCALL3(env, CallStaticVoidMethod, gNativeCallbacksClass, gNativeCallbacks_openFile, str);
    // smug_assert(gOpeningFile != NULL);
    if (gOpeningFile == NULL)
    {
        WARNING("Could not open file!");
    }
    else
    {
        NOTIFY("File opened!");
    }
    File* retFile = gOpeningFile;
    gOpeningFile = NULL;
    return retFile;
}

int File_fclose(File* self)
{
    free(self->mData);
    return 0;
}

SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeOpenFile)
  (JNIEnv* env, jobject thiz, jbyteArray jdata)
{
    // TODO: Error checking, returning null on fail.
    long int length = (long int)JCALL1(env, GetArrayLength, jdata);
    jboolean isCopy;
    unsigned char* data = (unsigned char*)JCALL2(env, GetByteArrayElements, jdata, &isCopy);
    if (isCopy)
    {
        /* JNI_COMMIT copies back the contents of data to jdata, but doesn't free data. This should
        be cheaper than the case below. */
        JCALL3(env, ReleaseByteArrayElements, jdata, (jbyte*)data, JNI_COMMIT);
        gOpeningFile = File_new(data, length);
    }
    else
    {
        /* JNI_ABORT frees data, and does not copy back data to jdata. */
        unsigned char* dataCopy = malloc(sizeof(char) * length);
        memcpy(dataCopy, data, length);
        JCALL3(env, ReleaseByteArrayElements, jdata, (jbyte*)data, JNI_ABORT);
        gOpeningFile = File_new(dataCopy, length);
    }
}

unsigned char* File_getBuffer(File* self)
{
    smug_assert(self != NULL);
    return self->mData;
}

void File_freeBuffer(File* self, unsigned char* buffer)
{
    smug_assert(self != NULL);
    // NOOP
}

size_t File_fread(File* self, void* ptr, size_t size, size_t count)
{
    smug_assert(self != NULL);
    long int read = min(count * size, self->mLength - self->mPosition);
    memcpy(ptr, self->mData + self->mPosition, read);
    return read;
}

int File_fseek(File* self, long int offset, int origin)
{
    smug_assert(self != NULL);
    switch (origin)
    {
        case SMUG_SEEK_SET:
            smug_assert(offset >= 0 && offset <= self->mLength);
            self->mPosition = offset;
            break;
        case SMUG_SEEK_CUR:
            smug_assert(self->mPosition + offset >= 0 && self->mPosition + offset <= self->mLength);
            self->mPosition += offset;
            break;
        case SMUG_SEEK_END:
            smug_assert(offset <= 0 && self->mLength + offset >= 0);
            self->mPosition = self->mLength + offset;
            break;
        default:
            smug_assert(!"Invalid offset parameter to File_fseek!");
            return 1;
    }
    return 0;
}

long int File_ftell(File* self)
{
    smug_assert(self != NULL);
    return self->mPosition;
}

long int File_getLength(File* self)
{
    smug_assert(self != NULL);
    return self->mLength;
}

/* Other platform functions */

int Platform_init(int width, int height, BOOL fullscreen)
{
    smug_assert(!isInitialized);
    NOTIFY("Initializing platform layer");

    setWindowSize(width, height);

    JNIEnv* env = getJniEnvironment();
    gNativeCallbacksClass = JCALL1(env, FindClass, "se/lolektivet/droidsmug/NativeCallbacks");
    gNativeCallbacks_openFile = JCALL3(env, GetStaticMethodID, gNativeCallbacksClass, "openFile", "(Ljava/lang/String;)V");
    gNativeCallbacks_changeFps = JCALL3(env, GetStaticMethodID, gNativeCallbacksClass, "changeFps", "(F)V");

    Platform_stepDiscreteTime();

    gLastFpsCheck = Platform_getTime();

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
    if (Platform_getTime() - gLastFpsCheck >= gFpsCheckInterval)
    {
        smug_printf("FPS: %i", (int)(((double)gFpsCounter) / gFpsCheckInterval));
        gLastFpsCheck = Platform_getTime();
        gFpsCounter = 0;
    }
    if (gLogicCallbackEnabled && gUserLogicCallback != NULL)
    {
        gUserLogicCallback();
    }
    if(Input_getKey(KEY_ESC) || !Platform_isWindowOpen())
    {
        Signal_send(SIG_EXIT);
    }
}

void Platform_setKillCallback(void(*callback)(void))
{
    gUserKillCallback = callback;
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
    gNativeCallbacksClass = JCALL1(env, FindClass, "se/lolektivet/droidsmug/NativeCallbacks");
    JCALL3(env, CallStaticVoidMethod, gNativeCallbacksClass, gNativeCallbacks_changeFps, (jfloat)fps);
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

float Platform_getInterpolationFactor(void)
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
    Log_setFormatString(LOG_NOTIFICATION, "%level% - %message% [%file%:%line%]");
    NOTIFY("Hello smug world!");
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeRender
 * Signature: ()V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeRender)
  (JNIEnv* env, jclass clazz)
{
    gFpsCounter++;
    Platform_stepDiscreteTime();
    gInterpolationFactor = (gDiscreteTime - gLastHeartbeat) / (1.0 / gFps);
    Graphics_render();
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeDeInit
 * Signature: ()V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeDeInit)
  (JNIEnv* env, jclass clazz)
{
    if (gUserKillCallback != NULL)
    {
        gUserKillCallback();
    }
    Engine_terminate();
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

