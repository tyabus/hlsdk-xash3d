#!/bin/sh
mkdir -p assets
python makepak.py pak/ assets/extras.pak
ndk-build NDK_TOOLCHAIN_VERSION=4.9 NDK_DEBUG=1 V=1 -j5 APP_CFLAGS="-w"
ant debug
#jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore ./ggm.keystore bin/ggm-debug.apk ggm
#zipalign 4 bin/cs16-client-unsigned.apk bin/mod.apk
