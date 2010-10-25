# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AM_CFLAGS =

SUBDIRS = utils common platform graphics physics input logic engine

lib_LTLIBRARIES = libsmug.la


libsmug_ladir = $(includedir)/smug

nobase_libsmug_la_HEADERS = smug.h \
common/interface.h \
engine/interface.h \
graphics/interface.h \
physics/interface.h \
utils/interface.h \
platform/interface.h \
input/interface.h \
logic/interface.h


#It kind of matters a whole lot which order these are in, reverse dependecy order seems to work

libsmug_la_SOURCES =
libsmug_la_LIBADD = engine/libsmug.la
#libsmug_la_LIBADD += graphics/libgraphics.la
#libsmug_la_LIBADD += physics/libphysics.la
#libsmug_la_LIBADD += logic/liblogic.la
#libsmug_la_LIBADD += input/libinput.la
#libsmug_la_LIBADD += platform/libplatform.la
#libsmug_la_LIBADD += utils/libutils.la
#libsmug_la_LIBADD += common/libcommon.la
