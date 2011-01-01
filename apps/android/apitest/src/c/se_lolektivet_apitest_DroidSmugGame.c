#include <smug.h>
#include <jni.h>
#include <math.h>

static SmugObject obj;

void logicCallback(void)
{
    double t = smugGetTime();
    double f = sin(t) * 50;
    // smug_printf("Gameobject position: %f, %f. Time: %f, f: %f", Vector_getX(&v), Vector_getY(&v), t, f);
    smugObject_setPos(obj, f, 0);
    // GameObject_setOpacity(obj, ((f + 1.0) / 4.0) + 0.5);
}

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeGameInit
  (JNIEnv* env, jobject thiz)
{
    // No need to call smugInit() here, since DroidSmug does that for you earlier.
    smugLogSetLevel(SMUG_LOG_LEVEL_ALL);
    obj = smugObject_new();
    SmugDrawable d = smugDrawable_newFromRect(smugRect_Create(200.0f, 200.0f, 40.0f, 40.0f));
    smugDrawable_setColor(d, smugColor_create(255, 0, 0, 255));
    smugObject_addDrawable(obj, d);
    smugAddObject(obj);
    smugSetLogicCallback(logicCallback);
    // smugDestroyObject(obj);
}
