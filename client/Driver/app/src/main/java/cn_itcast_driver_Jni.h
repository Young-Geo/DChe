/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class cn_itcast_driver_Jni */

#ifndef _Included_cn_itcast_driver_Jni
#define _Included_cn_itcast_driver_Jni
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cn_itcast_driver_Jni
 * Method:    Login
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_cn_itcast_driver_Jni_Login
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     cn_itcast_driver_Jni
 * Method:    SetStatus
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_cn_itcast_driver_Jni_SetStatus
  (JNIEnv *, jobject, jint);

/*
 * Class:     cn_itcast_driver_Jni
 * Method:    UpdateGeoInfo
 * Signature: (DD)I
 */
JNIEXPORT jint JNICALL Java_cn_itcast_driver_Jni_UpdateGeoInfo
  (JNIEnv *, jobject, jdouble, jdouble);

#ifdef __cplusplus
}
#endif
#endif
