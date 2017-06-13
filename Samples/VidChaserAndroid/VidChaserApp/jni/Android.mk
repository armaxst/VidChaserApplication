LOCAL_PATH := $(call my-dir)

VIDCHASER_APP_LOCAL_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)
include $(LOCAL_PATH)/../../../../Build/lib/Android/Android_prebuilt.mk

include $(CLEAR_VARS)
LOCAL_PATH := $(VIDCHASER_APP_LOCAL_PATH)
LOCAL_MODULE := VidChaserApp

RENDERER_SRC_PATH := ../../../../Renderer
RENDERER_SRCS := \
    $(RENDERER_SRC_PATH)/ProjectionMatrix.cpp \
    $(RENDERER_SRC_PATH)/RendererAPI.cpp \
    $(RENDERER_SRC_PATH)/RenderManager.cpp \
    $(RENDERER_SRC_PATH)/ShaderUtil.cpp \
    $(RENDERER_SRC_PATH)/Sticker.cpp

LOCAL_SRC_FILES := \
	$(RENDERER_SRCS) \
	VidChaserRenderer.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../Build/include $(LOCAL_PATH)/../../../../Renderer
LOCAL_LDLIBS := -llog -lGLESv2
LOCAL_CFLAGS += -std=c++11
LOCAL_SHARED_LIBRARIES := VidChaser-prebuilt MaxstAR-prebuilt
include $(BUILD_SHARED_LIBRARY)