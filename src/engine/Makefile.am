# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AM_CFLAGS = $(LIBGLFWCFLAGS) -I.. -I../common

noinst_LTLIBRARIES = libsmug.la

libsmug_la_SOURCES =  interface.c interface.h\
engine.c engine.h\
gameobject.c gameobject.h\
positionedobject.c positionedobject.h\
interpoint.c interpoint.h\
blocking_engine.c blocking_engine.h

# world.c world.h

libsmug_la_LIBADD = ../common/libcommon.la
libsmug_la_LIBADD += ../utils/libutils.la
libsmug_la_LIBADD += ../graphics/libgraphics.la
libsmug_la_LIBADD += ../physics/libphysics.la
libsmug_la_LIBADD += ../platform/libplatform.la
libsmug_la_LIBADD += ../input/libinput.la
libsmug_la_LIBADD += ../logic/liblogic.la
