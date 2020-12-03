#HLSDK server Android port
#Copyright (c) nicknekit

LOCAL_PATH := $(call my-dir)

GGM_COMMIT := $(firstword $(shell cd $(LOCAL_PATH)&&git rev-parse --short=6 HEAD) unknown)

include $(CLEAR_VARS)

LOCAL_MODULE := server

include $(XASH3D_CONFIG)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libserver_hardfp
endif

LOCAL_CFLAGS += -DGGM_COMMIT=\"$(GGM_COMMIT)\" -DCXX=\"$(CXX)\" -D_LINUX -DCLIENT_WEAPONS -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -D_snprintf=snprintf \
	-fno-exceptions -fvisibility=hidden -DNO_VOICEGAMEMGR -Wno-conversion-null -pipe -Wno-write-strings -std=gnu++98 -Wno-invalid-offsetof

LOCAL_C_INCLUDES := $(LOCAL_PATH)/. \
		    $(LOCAL_PATH)/wpn_shared \
		    $(LOCAL_PATH)/../common \
		    $(LOCAL_PATH)/../engine \
		    $(LOCAL_PATH)/../public \
		    $(LOCAL_PATH)/../pm_shared \
		    $(LOCAL_PATH)/../game_shared

LOCAL_SRC_FILES := agrunt.cpp \
	   airtank.cpp \
           aflock.cpp \
           animating.cpp \
           animation.cpp \
           apache.cpp \
           barnacle.cpp \
           barney.cpp \
           bigmomma.cpp \
           bmodels.cpp \
           bullsquid.cpp \
           buttons.cpp \
           cbase.cpp \
           client.cpp \
           combat.cpp \
           controller.cpp \
           crossbow.cpp \
	   BMOD_flyingcrowbar.cpp \
           crowbar.cpp \
           defaultai.cpp \
           doors.cpp \
           effects.cpp \
           egon.cpp \
           explode.cpp \
	   enttools.cpp \
           flyingmonster.cpp \
           func_break.cpp \
           func_tank.cpp \
           game.cpp \
           gamerules.cpp \
           gargantua.cpp \
           gauss.cpp \
           genericmonster.cpp \
           ggrenade.cpp \
           globals.cpp \
           glock.cpp \
           gman.cpp \
           h_ai.cpp \
           h_battery.cpp \
           h_cycler.cpp \
           h_export.cpp \
           handgrenade.cpp \
           hassassin.cpp \
           headcrab.cpp \
           healthkit.cpp \
           hgrunt.cpp \
           hornet.cpp \
           hornetgun.cpp \
           houndeye.cpp \
           ichthyosaur.cpp \
           islave.cpp \
           items.cpp \
           leech.cpp \
           lights.cpp \
           maprules.cpp \
           monstermaker.cpp \
           monsters.cpp \
           monsterstate.cpp \
           mortar.cpp \
           mp5.cpp \
           multiplay_gamerules.cpp \
           nihilanth.cpp \
           nodes.cpp \
	   observer.cpp \
           osprey.cpp \
           pathcorner.cpp \
           plane.cpp \
           plats.cpp \
           player.cpp \
	   playermonster.cpp \
           python.cpp \
           rat.cpp \
           roach.cpp \
           rpg.cpp \
	   satchel.cpp \
           schedule.cpp \
           scientist.cpp \
           scripted.cpp \
           shotgun.cpp \
           singleplay_gamerules.cpp \
           skill.cpp \
           sound.cpp \
           soundent.cpp \
           spectator.cpp \
           squadmonster.cpp \
           squeakgrenade.cpp \
           subs.cpp \
           talkmonster.cpp \
           teamplay_gamerules.cpp \
           tentacle.cpp \
           triggers.cpp \
           tripmine.cpp \
           turret.cpp \
           util.cpp \
           weapons.cpp \
           world.cpp \
           xen.cpp \
           zombie.cpp \
           prop.cpp \
	   cleansuit_scientist.cpp \
	   skeleton.cpp \
	   xtree.cpp \
	   creeper.cpp \
	   houndeye_dead.cpp \
	   h_player.cpp \
	   zombie_barney.cpp \
	   poisoncrab.cpp \
           gravgun.cpp \
	   shock.cpp \
	   shockrifle.cpp \
           ar2.cpp \
	   m249.cpp \
	   knife.cpp \
           big_cock.cpp \
	   agtimer.cpp \
           unpredictedweapon.cpp \
	   ../pm_shared/pm_debug.cpp \
	   ../pm_shared/pm_math.cpp \
	   ../pm_shared/pm_shared.cpp \
	   coop.cpp \
	   gateofbabylon.cpp \
	   gravgunmod.cpp \
	   admin.cpp \
#	   ../game_shared/voice_gamemgr.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

