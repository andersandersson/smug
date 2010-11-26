#include <jni.h>
#include <smug.h>

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeInit
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeInit
  (JNIEnv* env, jclass clazz, jfloat fps)
{
    smugInit();
    smugPrintf("Hello smug world!");
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeResize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeResize
  (JNIEnv* env, jclass clazz, jint width, jint height)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeRender
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeRender
  (JNIEnv* env, jclass clazz)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeDone
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeDone
  (JNIEnv* env, jclass clazz)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeTouchDown
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeTouchDown
  (JNIEnv* env, jclass clazz)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeTouchUp
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeTouchUp
  (JNIEnv* env, jclass clazz)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeKeyDown
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeKeyDown
  (JNIEnv* env, jclass clazz, jint keyCode)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeKeyUp
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeKeyUp
  (JNIEnv* env, jclass clazz, jint keyCode)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeOrientationChange
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeOrientationChange
  (JNIEnv* env, jclass clazz, jint orientation)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeHeartbeat
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_se_lolektivet_apitest_NativeFunctions_nativeHeartbeat
  (JNIEnv* env, jclass clazz)
{
}
