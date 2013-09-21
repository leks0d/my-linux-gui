# Copyright (C) 2013 Mango
#

LOCAL_PATH := $(call my-dir)
MY_ANDROID_SOURCE:=/home/liuwenlong/t80/android

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
LOCAL_MODULE := libFFmpegForId3-prebuild
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libFFmpegForId3.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libskia-prebuild
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libskia.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include

LOCAL_MODULE    := MangoPlayer
LOCAL_SRC_FILES := player.cpp
LOCAL_SRC_FILES += mediaView.cpp
LOCAL_SRC_FILES += playingView.cpp
LOCAL_SRC_FILES += SettingsView.cpp
LOCAL_SRC_FILES += MediaPlayerInterface.cpp
LOCAL_SRC_FILES += MedaiPlayerService.cpp
LOCAL_SRC_FILES += mediaprovider.cpp
LOCAL_SRC_FILES += playinglist.cpp
LOCAL_SRC_FILES += ArrayMediaInfo.cpp
LOCAL_SRC_FILES += MusicInfoView.cpp
LOCAL_SRC_FILES += DisplaySettingView.cpp
LOCAL_SRC_FILES += PointDrawView.cpp
LOCAL_SRC_FILES += VolumeView.cpp
LOCAL_SRC_FILES += SystemInfoView.cpp
LOCAL_SRC_FILES += EqSettingView.cpp
LOCAL_SRC_FILES += SettingProvider.cpp
LOCAL_SRC_FILES += PowerManager.cpp
LOCAL_SRC_FILES += ShutDownView.cpp
LOCAL_SRC_FILES += SocketDetect.cpp
LOCAL_SRC_FILES += UsmConnectView.cpp
LOCAL_SRC_FILES += SdcardInsertView.cpp
LOCAL_SRC_FILES += MediaScannerView.cpp
LOCAL_SRC_FILES += AlarmManager.cpp
LOCAL_SRC_FILES += BitmapFactory.cpp
LOCAL_SRC_FILES += MusicOperateView.cpp
LOCAL_SRC_FILES += KeyLockView.cpp
LOCAL_SRC_FILES += ChosenView.cpp
LOCAL_SRC_FILES += playlist.cpp
LOCAL_SRC_FILES += CCue.cpp
LOCAL_SRC_FILES += PlaylistOperateView.cpp

LOCAL_CPPFLAGS:=\
-I $(MY_ANDROID_SOURCE)/frameworks/base/core/jni/android/graphics\
-I $(MY_ANDROID_SOURCE)/external/skia/include/core\
-I $(MY_ANDROID_SOURCE)/external/skia/include/images\
-I $(MY_ANDROID_SOURCE)/frameworks/base/include\
-I $(MY_ANDROID_SOURCE)/system/core/include\
-I $(LOCAL_PATH)/ffmpeg/include

LOCAL_SHARED_LIBRARIES := libParticle-prebuild
LOCAL_SHARED_LIBRARIES += libParticleOmxil-prebuild
LOCAL_SHARED_LIBRARIES += libParticleOmxal-prebuild
LOCAL_SHARED_LIBRARIES += libFFmpegForId3-prebuild
LOCAL_SHARED_LIBRARIES += libskia-prebuild
APP_STL := stlport_static
LOCAL_LDLIBS :=  -llog -lcutils -licuuc -licui18n -lutils -lsqlite -L/home/liuwenlong/backup/lib


LOCAL_STATIC_LIBRARIES := Mango


include $(BUILD_EXECUTABLE)
