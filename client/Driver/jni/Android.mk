
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := libcurl.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
# libmyjni.so
LOCAL_MODULE := myjni
LOCAL_SRC_FILES := cJSON.c  CurlWrap.cpp  jni.cpp  JniUtil.cpp  JsonWrap.cpp  User.cpp GeoHash.cpp
LOCAL_LDLIBS := -llog -lz
LOCAL_STATIC_LIBRARIES := libcurl
include $(BUILD_SHARED_LIBRARY)

