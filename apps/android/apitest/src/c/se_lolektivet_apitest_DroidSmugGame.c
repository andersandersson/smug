#include <smug.h>
#include <jni.h>
#include <android/log.h>

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeGameInit
  (JNIEnv* env, jobject thiz)
{
    smugPrintf("Hello smug game!");
}
