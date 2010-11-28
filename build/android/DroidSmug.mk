SMUG_DEBUG ?= true

SMUG_ROOT := $(shell cygpath "$(call my-dir)/../..")

$(info SMUG_ROOT is '$(SMUG_ROOT)')

LOCAL_PATH := $(SMUG_ROOT)/src

$(info LOCAL_PATH is "$(LOCAL_PATH)")

include $(CLEAR_VARS)

LOCAL_MODULE := smug

LOCAL_CFLAGS := -DANDROID_NDK -DHAS_CONFIG_H -Wall -pedantic -std=c99 -Wno-variadic-macros

ifeq ($(SMUG_DEBUG),true)
# debug defines
endif

LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/platform/android \
    $(LOCAL_PATH) .

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
    platform/android/smugstd.c \
    graphics/graphics.c \
    graphics/texture/texture.c \
    graphics/renderer/renderer.c \
    graphics/renderer/renderbatch.c \
    graphics/renderer/batchdata.c \
    platform/android/physics_debug.c
    # platform/platform.c               #depends on GLFW
    # platform/android/platform.c \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)
