SMUG_DEBUG ?= true

SMUG_ROOT := $(shell cygpath "$(call my-dir)/../..")

$(info SMUG_ROOT is '$(SMUG_ROOT)')

LOCAL_PATH := $(SMUG_ROOT)/src

$(info LOCAL_PATH is "$(LOCAL_PATH)")

include $(CLEAR_VARS)

LOCAL_MODULE := smug

LOCAL_CFLAGS := -DANDROID_NDK -DHAS_CONFIG_H -Wall -std=c99 -Wno-variadic-macros -Werror=strict-prototypes

ifeq ($(SMUG_DEBUG),true)
# debug defines
LOCAL_CFLAGS := $(LOCAL_CFLAGS) -DSMUG_DEBUG
endif

LOCAL_ALLOW_UNDEFINED_SYMBOLS := false

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/platform/android \
    $(LOCAL_PATH) .

LOCAL_SRC_FILES := \
    common/interface.c \
    common/common.c \
    common/log.c \
    common/signal.c \
    engine/interface.c \
    engine/engine.c \
    engine/gameobject.c \
    engine/gameobjectiterator.c \
    engine/positionedobject.c \
    engine/interpoint.c \
    graphics/interface.c \
    graphics/camera.c \
    graphics/color.c \
    graphics/graphics.c \
    graphics/sprite.c \
    graphics/drawable/box.c \
    graphics/drawable/drawable.c \
    graphics/drawable/drawableshape.c \
    graphics/image/image.c \
    graphics/image/lodepng/lodepng.c \
    graphics/renderer/layer.c \
    graphics/renderer/renderer.c \
    graphics/renderer/renderbatch.c \
    graphics/renderer/batchdata.c \
    graphics/texture/texture.c \
    input/interface.c \
    input/input.c \
    input/controller.c \
    input/controllertype.c \
    physics/interface.c \
    physics/physics.c \
    physics/body.c \
    physics/calc.c \
    physics/waypoint.c \
    utils/interface.c \
    utils/arraylist.c \
    utils/binarysearchtree.c \
    utils/hook.c \
    utils/linkedlist.c \
    utils/linkedlistiterator.c \
    utils/map.c \
    utils/orderedset.c \
    utils/pair.c \
    utils/pathname.c \
    utils/point.c \
    utils/rectangle.c \
    utils/shapes.c \
    utils/sstring.c \
    utils/vector.c \
    platform/interface.c \
    platform/android/platform/opengl/importgl.c \
    platform/android/console.c \
    platform/android/input.c \
    platform/android/platform.c \
    platform/android/smugstd.c \
    platform/android/threads.c \
    platform/android/physics_debug.c
    # graphics/drawable/drawable.c \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)
