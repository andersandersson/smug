// #include <smug.h>

#include <jni.h>
#include <android/log.h>

JNIEXPORT void JNICALL Java_se_lolektivet_apitest_DroidSmugGame_nativeGameInit
  (JNIEnv* env, jobject thiz)
{
    __android_log_write(ANDROID_LOG_INFO, "DroidSmugGame", "Hello smug game!");
}
