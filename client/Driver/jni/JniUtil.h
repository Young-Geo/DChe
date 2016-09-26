//
// Created by xueguoliang on 16-9-24.
//

#ifndef DRIVER_JNIUTIL_H
#define DRIVER_JNIUTIL_H

#include <android/log.h>
#include <jni.h>

static const char* tag = "DirverDebugTag";

#define MyErr(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#define MyWarning(fmt, ...) __android_log_print(ANDROID_LOG_WARN, tag, fmt, ##__VA_ARGS__)
#define MyInfo(fmt, ...) __android_log_print(ANDROID_LOG_INFO, tag, fmt, ##__VA_ARGS__)
#define MyDebug(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)

class JniUtil
{
public:
    static char* JString2CStr(JNIEnv* env, jstring jstr);
    static jstring CStr2JString(JNIEnv* env, const char* cstr);
    static void DeleteCStr(char* cstr);
    static char* convert(int value, char* buf);
    static char* convert(double value, char* buf);
};



#endif //DRIVER_JNIUTIL_H
