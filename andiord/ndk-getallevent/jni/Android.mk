LOCAL_PATH:=$(call my-dir)/..
include $(CLEAR_VARS)

LOCAL_SRC_FILES := main.c

LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := getallevent
include $(BUILD_EXECUTABLE)
