SMUG_ROOT ?= $(shell cygpath "$(call my-dir)/..")

include $(SMUG_ROOT)/build/android/jni/Android.mk

include $(CLEAR_VARS)

SMUG_APP_SRC := ../apps/android/apitest/src/c

LOCAL_MODULE := apitest

LOCAL_CFLAGS := -DANDROID_NDK -DHAS_CONFIG_H -Wall -pedantic -std=c99 -Wno-variadic-macros

LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

LOCAL_SHARED_LIBRARIES := smug

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/platform/android \
    $(LOCAL_PATH)

LOCAL_SRC_FILES := $(SMUG_APP_SRC)/se_lolektivet_apitest_NativeFunctions.c

include $(BUILD_SHARED_LIBRARY)
