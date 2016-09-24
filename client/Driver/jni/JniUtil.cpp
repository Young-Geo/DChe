//
// Created by xueguoliang on 16-9-24.
//

#include "JniUtil.h"
#include <string.h>

char *JniUtil::JString2CStr(JNIEnv *env, jstring str)
{
    char* pStr = NULL;

    jclass strClass = env->FindClass("java/lang/String");
    jstring encode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(strClass, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray data = (jbyteArray)env->CallObjectMethod(str, mid, encode);
    jsize len = env->GetArrayLength(data);
    jbyte* buf = env->GetByteArrayElements(data, JNI_FALSE);

    if(len == 0)
       return NULL;

    pStr = new char[len+1];
    pStr[len] = 0;

    memcpy(pStr, buf, len);

    env->ReleaseByteArrayElements(data, buf, 0);
    return pStr;
}

jstring JniUtil::CStr2JString(JNIEnv *env, const char *cstr)
{
    return env->NewStringUTF(cstr);
}

void JniUtil::DeleteCStr(char *cstr)
{
    delete[] cstr;
}
