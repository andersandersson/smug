# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

SUBDIRS = . test

AM_CFLAGS = $(LIBGLFWCFLAGS) -I.. -I../common

noinst_LTLIBRARIES = libplatform.la

libplatform_la_SOURCES = interface.c interface.h\
threads.c threads.h\
platform.h platform.c\
console.c console.h\
smugstd.c smugstd.h\
file.c file.h
#opengl/opengl.h opengl/GLee.c opengl/GLee.h

