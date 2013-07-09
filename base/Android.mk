# Copyright (C) 2013 Mango
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/canvas/freetype_ndk/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/canvas/freetype_ndk/devel

LOCAL_MODULE    := Mango

LOCAL_SRC_FILES += canvas/freetype_ndk/src/expansion/ft_ex_heap.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/expansion/ft_ex_mem.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/expansion/ft_ex_truetype.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/base/ftbase.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/base/ftbbox.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/base/ftbitmap.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/base/ftdebug.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/base/ftglyph.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/smooth/ftgrays.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/base/ftinit.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/smooth/ftsmooth.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/base/ftsystem.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/psnames/psnames.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/sfnt/sfnt.c
LOCAL_SRC_FILES += canvas/freetype_ndk/src/truetype/truetype.c

LOCAL_SRC_FILES += canvas/bitBlt.cpp
LOCAL_SRC_FILES += canvas/bitmap.cpp
LOCAL_SRC_FILES += canvas/brush.cpp
LOCAL_SRC_FILES += canvas/canvas.cpp
LOCAL_SRC_FILES += canvas/canvasBase.cpp
LOCAL_SRC_FILES += canvas/fill.cpp
LOCAL_SRC_FILES += canvas/font.cpp
LOCAL_SRC_FILES += canvas/line.cpp
LOCAL_SRC_FILES += canvas/pen.cpp
LOCAL_SRC_FILES += canvas/region.cpp
LOCAL_SRC_FILES += canvas/text.cpp

LOCAL_SRC_FILES += util/point.cpp
LOCAL_SRC_FILES += util/rect.cpp
LOCAL_SRC_FILES += util/size.cpp
LOCAL_SRC_FILES += util/string.cpp

LOCAL_SRC_FILES += party/charset.cpp
LOCAL_SRC_FILES += party/condition.cpp
LOCAL_SRC_FILES += party/file.cpp
LOCAL_SRC_FILES += party/log.cpp
LOCAL_SRC_FILES += party/message.cpp
LOCAL_SRC_FILES += party/mutex.cpp
LOCAL_SRC_FILES += party/object.cpp
LOCAL_SRC_FILES += party/party.cpp
LOCAL_SRC_FILES += party/resource.cpp
LOCAL_SRC_FILES += party/semaphore.cpp
LOCAL_SRC_FILES += party/session.cpp
LOCAL_SRC_FILES += party/thread.cpp
LOCAL_SRC_FILES += party/time.cpp

LOCAL_SRC_FILES += view/button.cpp
LOCAL_SRC_FILES += view/listView.cpp
LOCAL_SRC_FILES += view/staticView.cpp
#LOCAL_SRC_FILES += view/trackbar.cpp
LOCAL_SRC_FILES += view/view.cpp
LOCAL_SRC_FILES += view/viewZAxis.cpp
LOCAL_SRC_FILES += view/TextView.cpp
LOCAL_SRC_FILES += view/ImageView.cpp
LOCAL_SRC_FILES += view/SeekBar.cpp

#LOCAL_LDLIBS :=  -llog 

include $(BUILD_STATIC_LIBRARY)
