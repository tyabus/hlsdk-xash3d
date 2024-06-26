#hlsdk-2.3 client port for android
#Copyright (c) mittorn

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := client

include $(XASH3D_CONFIG)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libclient_hardfp
endif

LOCAL_CFLAGS += -DCLIENT_DLL -D_LINUX -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -Wno-write-strings -std=gnu++0x -pipe

LOCAL_SRC_FILES := ev_hldm.cpp \
		hl/hl_baseentity.cpp \
		hl/hl_objects.cpp \
		hl/hl_weapons.cpp \
		hl/hl_events.cpp \
		ammo.cpp \
		ammo_secondary.cpp \
		ammohistory.cpp \
		battery.cpp \
		cdll_int.cpp \
		com_weapons.cpp \
		death.cpp \
		demo.cpp \
		entity.cpp \
		ev_common.cpp \
		events.cpp \
		flashlight.cpp \
		GameStudioModelRenderer.cpp \
		geiger.cpp \
		health.cpp \
		hud.cpp \
		hud_msg.cpp \
		hud_redraw.cpp \
		hud_spectator.cpp \
		hud_update.cpp \
		in_camera.cpp \
		input.cpp \
		input_mouse.cpp \
		menu.cpp \
		message.cpp \
		overview.cpp \
		parsemsg.cpp \
		../pm_shared/pm_math.c \
 		../pm_shared/pm_shared.c \
		saytext.cpp \
		status_icons.cpp \
		statusbar.cpp \
		studio_util.cpp \
		StudioModelRenderer.cpp \
		text_message.cpp \
		train.cpp \
		tri.cpp \
		util.cpp \
		view.cpp \
		input_xash3d.cpp \
		scoreboard.cpp \
		MOTD.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/. \
		 $(LOCAL_PATH)/../common \
		 $(LOCAL_PATH)/../engine \
		 $(LOCAL_PATH)/../game_shared \
		 $(LOCAL_PATH)/../dlls \
		 $(LOCAL_PATH)/../pm_shared

include $(BUILD_SHARED_LIBRARY)
