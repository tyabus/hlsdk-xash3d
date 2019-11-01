#hlsdk-2.3 client port for android
#Copyright (c) mittorn

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := client

include $(XASH3D_CONFIG)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libclient_hardfp
endif

LOCAL_CFLAGS += -DCLIENT_DLL -Wno-write-strings -DLINUX -D_LINUX -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DCLIENT_WEAPONS -w

LOCAL_SRC_FILES := ../dlls/crossbow.cpp
LOCAL_SRC_FILES += ../dlls/crowbar.cpp
LOCAL_SRC_FILES += ../dlls/egon.cpp
LOCAL_SRC_FILES += ev_hldm.cpp
LOCAL_SRC_FILES += ../dlls/gauss.cpp
LOCAL_SRC_FILES += ../dlls/handgrenade.cpp
LOCAL_SRC_FILES += hl/hl_baseentity.cpp
LOCAL_SRC_FILES += hl/hl_events.cpp
LOCAL_SRC_FILES += hl/hl_objects.cpp
LOCAL_SRC_FILES += hl/hl_weapons.cpp
LOCAL_SRC_FILES += ../dlls/hornetgun.cpp
LOCAL_SRC_FILES += ../dlls/mp5.cpp
LOCAL_SRC_FILES += ../dlls/python.cpp
LOCAL_SRC_FILES += ../dlls/rpg.cpp
LOCAL_SRC_FILES += ../dlls/satchel.cpp
LOCAL_SRC_FILES += ../dlls/shotgun.cpp
LOCAL_SRC_FILES += ../dlls/squeakgrenade.cpp
LOCAL_SRC_FILES += ../dlls/tripmine.cpp
LOCAL_SRC_FILES += ../dlls/glock.cpp
#LOCAL_SRC_FILES += ../game_shared/voice_banmgr.cpp
#LOCAL_SRC_FILES += ../game_shared/voice_status.cpp
LOCAL_SRC_FILES += ammo.cpp
LOCAL_SRC_FILES += ammo_secondary.cpp
LOCAL_SRC_FILES += ammohistory.cpp
LOCAL_SRC_FILES += battery.cpp
LOCAL_SRC_FILES += cdll_int.cpp
LOCAL_SRC_FILES += com_weapons.cpp
LOCAL_SRC_FILES += death.cpp
LOCAL_SRC_FILES += demo.cpp
LOCAL_SRC_FILES += entity.cpp
LOCAL_SRC_FILES += ev_common.cpp
LOCAL_SRC_FILES += events.cpp
LOCAL_SRC_FILES += flashlight.cpp
LOCAL_SRC_FILES += GameStudioModelRenderer.cpp
LOCAL_SRC_FILES += geiger.cpp
LOCAL_SRC_FILES += health.cpp
LOCAL_SRC_FILES += hud.cpp
LOCAL_SRC_FILES += hud_msg.cpp
LOCAL_SRC_FILES += hud_redraw.cpp
LOCAL_SRC_FILES += hud_spectator.cpp
LOCAL_SRC_FILES += hud_update.cpp
LOCAL_SRC_FILES += in_camera.cpp
LOCAL_SRC_FILES += input.cpp
LOCAL_SRC_FILES += input_mouse.cpp
#LOCAL_SRC_FILES += inputw32.cpp
LOCAL_SRC_FILES += menu.cpp
LOCAL_SRC_FILES += message.cpp
LOCAL_SRC_FILES += overview.cpp
LOCAL_SRC_FILES += parsemsg.cpp
LOCAL_SRC_FILES += ../pm_shared/pm_debug.c
LOCAL_SRC_FILES += ../pm_shared/pm_math.c
LOCAL_SRC_FILES += ../pm_shared/pm_shared.c
LOCAL_SRC_FILES += saytext.cpp
LOCAL_SRC_FILES += status_icons.cpp
LOCAL_SRC_FILES += statusbar.cpp
LOCAL_SRC_FILES += studio_util.cpp
LOCAL_SRC_FILES += StudioModelRenderer.cpp
LOCAL_SRC_FILES += text_message.cpp
LOCAL_SRC_FILES += train.cpp
LOCAL_SRC_FILES += tri.cpp
LOCAL_SRC_FILES += util.cpp
LOCAL_SRC_FILES += view.cpp
LOCAL_SRC_FILES += input_xash3d.cpp
LOCAL_SRC_FILES += scoreboard.cpp
LOCAL_SRC_FILES += MOTD.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/. \
		 $(LOCAL_PATH)/../common \
		 $(LOCAL_PATH)/../engine \
		 $(LOCAL_PATH)/../game_shared \
		 $(LOCAL_PATH)/../dlls \
		 $(LOCAL_PATH)/../pm_shared \
		 $(LOCAL_PATH)/../utils/false_vgui/include

include $(BUILD_SHARED_LIBRARY)
