#include <smug.h>
#include <jni.h>
#include <math.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

static SmugObject obj;
static SmugObject avatar;
static BOOL fingerAvatarConnection = FALSE;
static BOOL moveAvatarTowardsFinger = FALSE;
static float lastFingerX;
static float lastFingerY;

static const float fingerNearAvatarThreshhold = 20.0f;
static const float minX = 20.0f;
static const float maxX = 460.0f;
static const float avatarSpeed = 200.0f;
static const float fps = 20.0f;

void logicCallback(void)
{
    double t = smugGetTime();
    double f = sin(t) * 50;
    // smug_printf("Gameobject position: %f, %f. Time: %f, f: %f", Vector_getX(&v), Vector_getY(&v), t, f);
    smugObject_setPos(obj, f, 0);
    // GameObject_setOpacity(obj, ((f + 1.0) / 4.0) + 0.5);

    if (moveAvatarTowardsFinger)
    {
        float x = smugObject_getX(avatar);
        float dir = 0.0f;
        if (lastFingerX < x)
        {
            dir = -1.0f;
        }
        else
        {
            dir = 1.0f;
        }
        smugObject_setPos(avatar, x + dir * (avatarSpeed / fps), smugObject_getY(avatar));
    }
}

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeGameInit
  (JNIEnv* env, jobject thiz)
{
    // No need to call smugInit() here, since DroidSmug does that for you earlier.
    smugLogSetLevel(SMUG_LOG_LEVEL_ALL);
    smugSetLogicCallback(logicCallback);
    smugSetLogicFps(fps);

    obj = smugObject_new();
    SmugDrawable d = smugDrawable_newFromRect(smugRect_create(200.0f, 200.0f, 40.0f, 40.0f));
    smugDrawable_setColor(d, smugColor_create(255, 0, 0, 255));
    smugObject_addDrawable(obj, d);
    smugAddObject(obj);

    avatar = smugObject_new();
    d = smugDrawable_newFromRect(smugRect_create(-20.0f, -40.0f, 40.0f, 40.0f));
    smugDrawable_setColor(d, smugColor_create(255, 255, 0, 255));
    smugObject_addDrawable(avatar, d);
    smugObject_setPos(avatar, 20.0f, 240.0f);
    smugAddObject(avatar);
    // smugDestroyObject(obj);
}

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeTouchDown
  (JNIEnv* env, jobject thiz, jfloat x, jfloat y)
{
    if (fabs(x - smugObject_getX(avatar)) <= fingerNearAvatarThreshhold)
    {
        fingerAvatarConnection = TRUE;
    }
    else
    {
        moveAvatarTowardsFinger = TRUE;
        lastFingerX = x;
        lastFingerY = y;
    }
}

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeTouchMove
  (JNIEnv* env, jobject thiz, jfloat x, jfloat y)
{
    if (fingerAvatarConnection)
    {
        smugObject_setPos(avatar, MAX(minX, MIN(maxX, x)), 240.0f);
    }
    else if (fabs(x - smugObject_getX(avatar)) <= fingerNearAvatarThreshhold)
    {
        fingerAvatarConnection = TRUE;
        moveAvatarTowardsFinger = FALSE;
    }
    else
    {
        lastFingerX = x;
        lastFingerY = y;
    }
}

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeTouchUp
  (JNIEnv* env, jobject thiz, jfloat x, jfloat y)
{
    moveAvatarTowardsFinger = FALSE;
    fingerAvatarConnection = FALSE;
}
