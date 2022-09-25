#
# Project:      Thirty Meter Telescope (TMT)
# System:       Primary Mirror Control System (M1CS)
# Task:         Network Communication Services (net)
# Module:       Network Communication Services Makefile
#
# Author:       T. Trinh, JPL, June 2021
#

TARGET_SYS=_am64x
ifeq ($(TARGET_SYS),_am335x)
    TOOLCHAIN_PREFIX=arm-linux-gnueabihf-
else ifeq ($(TARGET_SYS),_am64x)
    AM64X_SDK_PATH ?= /opt/ti/arago-2021.09-aarch64
    AM64X_SDK_BASE_SYSROOT=$(AM64X_SDK_PATH)/sysroots
    AM64X_SDK_TARGET_SYSROOT=$(AM64X_SDK_BASE_SYSROOT)/aarch64-linux
    AM64X_SDK_NATIVE_SYSROOT=$(AM64X_SDK_BASE_SYSROOT)/x86_64-arago-linux
    TOOLCHAIN_PREFIX=$(AM64X_SDK_NATIVE_SYSROOT)/usr/bin/aarch64-none-linux-gnu-
    SYSROOT=$(AM64X_SDK_TARGET_SYSROOT)
    CC_INCDIR=$(SYSROOT)/usr/include/
else
    TOOLCHAIN_PREFIX=
endif

# override default compiler tools to cross compile
CPP = $(TOOLCHAIN_PREFIX)gcc -E
CC = $(TOOLCHAIN_PREFIX)gcc
CXX = $(TOOLCHAIN_PREFIX)g++
LD = $(TOOLCHAIN_PREFIX)g++
AR = $(TOOLCHAIN_PREFIX)ar
RANLIB = $(TOOLCHAIN_PREFIX)ranlib
STRIP = $(TOOLCHAIN_PREFIX)strip
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
AS = $(TOOLCHAIN_PREFIX)as
AR = $(TOOLCHAIN_PREFIX)ar
NM = $(TOOLCHAIN_PREFIX)nm
GDB = $(TOOLCHAIN_PREFIX)gdb

# DEFINES: list of preprocessor flags to set when compiling source
DEFINES = -DLINUX

# CFLAGS: standard C compilier flags
CFLAGS = --sysroot=$(SYSROOT) -Wall -g -O -Wno-format-overflow

# CXXFLAGS: standard C++ compilier flags to set
CXXFLAGS = --sysroot=$(SYSROOT) -Wall -g -O -Wno-format-overflow -std=c++17

# LLIBS: local project libraries to link to EXE
LLIBS =

# LDLIBS: system libraries/library paths to link to EXE
LDLIBS = -lutil$(TARGET_SYS) -lpthread -lrt --sysroot=$(SYSROOT)

# EXES: name of executable(s) to be created.
EXES = lscs_udp$(TARGET_SYS) rtc_udp$(TARGET_SYS)
#EXES = rtc_tstcli$(TARGET_SYS)

# SRCS: list of source files to be compiled/linked with EXE.o
#SRCS = lscs_tstsrv.c rtc_tstcli.c
SRCS =  rtc_udp_am64x.cpp UdpConnection.cpp Server.cpp Client.cpp PThread.cpp lscs_udp_am64x.cpp


