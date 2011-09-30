#!/bin/sh
LOCAL_PATH=`pwd`
export NDK_ROOT=$LOCAL_PATH/../../../..
#PREBUILT=/home/yyin5/android-ndk-r6/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/
#PLATFORM=/home/yyin5/android-ndk-r6/platforms/android-4/arch-arm/
PREBUILT=$NDK_ROOT/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/
PLATFORM=$NDK_ROOT/platforms/android-4/arch-arm/

./configure --target-os=linux \
			--disable-logging \
			--arch=arm \
			--enable-version3 \
			--enable-gpl \
			--disable-nonfree \
			--disable-doc \
			--enable-stripping \
			--disable-ffmpeg \
			--disable-ffplay \
			--disable-ffserver \
			--disable-ffprobe \
			--disable-encoders \
			--disable-muxers \
			--disable-devices \
			--disable-protocols \
			--enable-protocol=file \
			--disable-avdevice \
			--enable-avcodec \
			--enable-avformat \
			--disable-postproc \
			--disable-w32threads \
			--disable-x11grab \
			--disable-network \
			--disable-gray	\
			--enable-avfilter \
			--disable-network \
			--enable-cross-compile \
			--disable-everything \
			--enable-decoder=h264 \
			--enable-parser=h264 \
			--enable-hwaccel=h264 \
			--enable-muxer=h264 \
			--enable-demuxer=h264 \
			--enable-bsf=h264 \
  			--enable-memalign-hack \
			--cc=$PREBUILT/bin/arm-linux-androideabi-gcc \
			--cross-prefix=$PREBUILT/bin/arm-linux-androideabi- \
			--nm=$PREBUILT/bin/arm-linux-androideabi-nm \
			--extra-cflags="-fPIC -DANDROID" \
			--disable-asm \
			--enable-neon \
			--disable-amd3dnow \
			--disable-amd3dnowext \
			--disable-altivec  \
  			--disable-mmx \
  			--disable-mmx2 \
  			--disable-sse  \
  			--disable-ssse3  \
  			--disable-avx    \
			--enable-sram	\
			--enable-armv5te \
			--disable-debug \
			--extra-ldflags="-Wl,-T,$PREBUILT/arm-linux-androideabi/lib/ldscripts/armelf_linux_eabi.x -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib $PREBUILT/lib/gcc/arm-linux-androideabi/4.4.3/crtbegin.o $PREBUILT/lib/gcc/arm-linux-androideabi/4.4.3/crtend.o -lc -lm -ldl"

sed -i 's/HAVE_LRINT 0/HAVE_LRINT 1/g' config.h 
sed -i 's/HAVE_LRINTF 0/HAVE_LRINTF 1/g' config.h  
sed -i 's/HAVE_ROUND 0/HAVE_ROUND 1/g' config.h  
sed -i 's/HAVE_ROUNDF 0/HAVE_ROUNDF 1/g' config.h  
sed -i 's/HAVE_TRUNC 0/HAVE_TRUNC 1/g' config.h  
sed -i 's/HAVE_TRUNCF 0/HAVE_TRUNCF 1/g' config.h
