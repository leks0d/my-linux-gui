# Copyright (C) 2013 Mango
#

#ALL_MAKEFILES := $(call all-subdir-makefiles)
#$(info all: $(ALL_MAKEFILES))
#include $(call all-subdir-makefiles)

MANGO_PATH := $(call my-dir)

include $(MANGO_PATH)/base/Android.mk
include $(MANGO_PATH)/apps/player/Android.mk



