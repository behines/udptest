#
# Project:	Thirty Meter Telescope (TMT)
# System:	Primary Mirror Control System (M1CS)
# Task:		Network Communication Services (net)
# Module:	Network Communication Services Makefile
#
# Author:	T. Trinh, JPL, June 2021
#

# Linux version
#
CC = gcc
CPP = cpp
LD = g++
AR = ar
CC_INCDIR = /usr/include
RANLIB =

DEFINES = -DLINUX
CFLAGS = -Wall -g -O -Wno-format-overflow
CXXFLAGS = -Wall -g -O -std=c++17
#

LLIBS =
LDLIBS = -lutil -lpthread -lrt

#

EXES = rtc_udp lscs_udp

SRCS = rtc_udp.cpp UdpConnection.cpp Server.cpp Client.cpp PThread.cpp lscs_udp.cpp

