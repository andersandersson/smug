V ?= 1
DEBUG ?= true

SMUG_ROOT := $(shell cygpath "$(call my-dir)/../../..")

#LOCAL_PATH := $(call my-dir)/src/jni
LOCAL_PATH := $(SMUG_ROOT)/src
#LOCAL_PATH := /cygdrive/d/Pyssel/Koda/jolivi/src/jni

$(info Local path is: '$(LOCAL_PATH)')

include $(CLEAR_VARS)

LOCAL_MODULE := smug

LOCAL_CFLAGS := -DANDROID_NDK -DHAS_CONFIG_H -Wall -pedantic -std=c99 -Wno-variadic-macros

ifeq ($(DEBUG),true)
# debug defines
endif

LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/platform/android \
    $(LOCAL_PATH) .
    # $(LOCAL_PATH)/common \
    # $(LOCAL_PATH)/engine \
    # $(LOCAL_PATH)/graphics \
    # $(LOCAL_PATH)/input \
    # $(LOCAL_PATH)/physics \
    # $(LOCAL_PATH)/platform \
    # $(LOCAL_PATH)/utils

LOCAL_SRC_FILES := \
    common/interface.c \
    common/common.c \
    common/console.c \
    common/log.c \
    engine/interface.c \
    engine/engine.c \
    engine/gameobject.c \
    graphics/interface.c \
    graphics/camera.c \
    graphics/color.c \
    graphics/sprite.c \
    graphics/drawable/box.c \
    graphics/drawable/drawable.c \
    graphics/image/image.c \
    graphics/image/lodepng/lodepng.c \
    graphics/renderer/layer.c \
    input/interface.c \
    input/input.c \
    input/controller.c \
    input/controllertype.c \
    physics/interface.c \
    physics/physics.c \
    physics/body.c \
    physics/shapes.c \
    utils/arraylist.c \
    utils/hook.c \
    utils/interface.c \
    utils/linkedlist.c \
    utils/map.c \
    utils/pathname.c \
    utils/point.c \
    utils/rectangle.c \
    utils/sstring.c \
    utils/vector.c \
    platform/android/platform/opengl/importgl.c \
    common/signal.c \
    platform/android/threads.c \
    platform/android/platform.c \
    platform/android/input.c \
    graphics/graphics.c \
    graphics/texture/texture.c \
    graphics/renderer/renderer.c \
    graphics/renderer/renderbatch.c \
    graphics/renderer/batchdata.c \
    physics/debug.c
    # platform/platform.c               #depends on GLFW
    # platform/android/platform.c \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)
