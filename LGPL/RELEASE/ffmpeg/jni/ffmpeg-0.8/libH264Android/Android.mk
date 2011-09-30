LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../av.mk
LOCAL_SRC_FILES := $(FFFILES)
LOCAL_C_INCLUDES :=  \
$(LOCAL_PATH)  \
$(LOCAL_PATH)/..
LOCAL_CFLAGS += $(FFCFLAGS)
LOCAL_LDLIBS := -lz
#LOCAL_SHARED_LIBRARIES := libavutil libavcodec libavformat
LOCAL_STATIC_LIBRARIES := libswscale libavcodec libavformat libavutil
#LOCAL_STATIC_LIBRARIES :=libswscale libavcodec libavutil libavformat # orginal
#LOCAL_STATIC_LIBRARIES := libavutil libavcodec libavformat
LOCAL_MODULE := $(FFNAME)
include $(BUILD_SHARED_LIBRARY)
