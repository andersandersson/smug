#include <jni.h>
#include <smug.h>

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeInit
 * Signature: (F)V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeInit)
  (JNIEnv* env, jclass clazz, jfloat fps)
{
    smugInit();
    smugPrintf("Hello smug world!");
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeRender
 * Signature: ()V
 */
SMUGEXPORT void JNICALL JAVA_IMPLEMENTATION(nativeRender)
  (JNIEnv* env, jclass clazz)
{
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
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeTouchUp
 * Signature: ()Z
 */
SMUGEXPORT jboolean JNICALL JAVA_IMPLEMENTATION(nativeTouchUp)
  (JNIEnv* env, jclass clazz)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeKeyDown
 * Signature: (I)Z
 */
SMUGEXPORT jboolean JNICALL JAVA_IMPLEMENTATION(nativeKeyDown)
  (JNIEnv* env, jclass clazz, jint keyCode)
{
}

/*
 * Class:     se_lolektivet_apitest_NativeFunctions
 * Method:    nativeKeyUp
 * Signature: (I)Z
 */
SMUGEXPORT jboolean JNICALL JAVA_IMPLEMENTATION(nativeKeyUp)
  (JNIEnv* env, jclass clazz, jint keyCode)
{
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
