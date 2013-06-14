# Copyright (C) 2013 Mango
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libParticle-prebuild
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libParticle.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libParticleOmxil-prebuild
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libParticleOmxil.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libParticleOmxal-prebuild
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libParticleOmxal.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include

LOCAL_MODULE    := MangoPlayer
LOCAL_SRC_FILES := player.cpp
LOCAL_SRC_FILES += mediaView.cpp
LOCAL_SRC_FILES += playingView.cpp
LOCAL_SRC_FILES += playlist.cpp
LOCAL_SRC_FILES += MediaPlayerInterface.cpp
LOCAL_SRC_FILES += MedaiPlayerService.cpp

LOCAL_SHARED_LIBRARIES := libParticle-prebuild
LOCAL_SHARED_LIBRARIES += libParticleOmxil-prebuild
LOCAL_SHARED_LIBRARIES += libParticleOmxal-prebuild

LOCAL_LDLIBS :=  -llog 
LOCAL_STATIC_LIBRARIES := Mango


include $(BUILD_EXECUTABLE)
