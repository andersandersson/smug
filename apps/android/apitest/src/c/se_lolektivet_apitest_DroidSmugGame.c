#include <smug.h>
#include <jni.h>
#include <math.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

static SmugObject obj1;
static SmugObject obj2;
static SmugObject avatar;
static BOOL fingerAvatarConnection = FALSE;
static BOOL moveAvatarTowardsFinger = FALSE;
static float lastFingerX;
static float lastFingerY;

static const float fingerNearAvatarThreshhold = 20.0f;
static const float minX = 20.0f;
static const float maxX = 460.0f;
static const float avatarSpeed = 400.0f;
static const float fps = 20.0f;

void logicCallback(void)
{
    double t = smugGetTime();
    double f = sin(t*2) * 200;
    // smug_printf("Gameobject position: %f, %f. Time: %f, f: %f", Vector_getX(&v), Vector_getY(&v), t, f);
    smugObject_moveTo(obj1, f, 0);
    smugObject_setPos(obj2, f, 0);
    // GameObject_setOpacity(obj, ((f + 1.0) / 4.0) + 0.5);

    if (moveAvatarTowardsFinger)
    {
        const float x = smugObject_getX(avatar);
        float dir = 0.0f;
        if (lastFingerX < x)
        {
            dir = -1.0f;
        }
        else
        {
            dir = 1.0f;
        }
        smugObject_moveTo(avatar, x + dir * (avatarSpeed / fps), smugObject_getY(avatar));
        if (fabs(lastFingerX - smugObject_getX(avatar)) <= fingerNearAvatarThreshhold)
        {
            fingerAvatarConnection = TRUE;
            moveAvatarTowardsFinger = FALSE;
        }
    }
}

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeGameInit
  (JNIEnv* env, jobject thiz)
{
    // No need to call smugInit() here, since DroidSmug does that for you earlier.
    smugLogSetLevel(SMUG_LOG_LEVEL_ALL);
    smugSetLogicCallback(logicCallback);
    smugSetLogicFps(fps);

    obj1 = smugObject_new();
    SmugDrawable d = smugDrawable_newFromRect(smugRect_create(0.0f, 0.0f, 40.0f, 40.0f));
    smugDrawable_setColor(d, smugColor_create(255, 0, 0, 255));
    smugObject_addDrawableAt(obj1, d, 240.0f, 100.0f);
    smugAddObject(obj1);

    obj2 = smugObject_new();
    d = smugDrawable_newFromRect(smugRect_create(0.0f, 0.0f, 40.0f, 40.0f));
    smugDrawable_setColor(d, smugColor_create(0, 255, 0, 255));
    smugObject_addDrawableAt(obj2, d, 240.0f, 60.0f);
    smugAddObject(obj2);

    avatar = smugObject_new();
    d = smugDrawable_newFromRect(smugRect_create(0.0f, 0.0f, 40.0f, 40.0f));
    smugDrawable_setColor(d, smugColor_create(255, 255, 0, 255));
    smugObject_addDrawableAt(avatar, d, -20.0f, -40.0f);
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
        smugObject_moveTo(avatar, MAX(minX, MIN(maxX, x)), 240.0f);
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
