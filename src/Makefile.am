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

SUBDIRS = utils common platform graphics physics script engine

bin_PROGRAMS = smug

smug_SOURCES = main.c

#It kind of matters a whole lot which order these are in, reverse dependecy order seems to work, FIX THIS!

smug_LDADD = script/libscript.la
smug_LDADD += engine/libsmug.la
smug_LDADD += common/libcommon.la
smug_LDADD += graphics/libgraphics.la
smug_LDADD += physics/libphysics.la
smug_LDADD += platform/libplatform.la
smug_LDADD += utils/libutils.la
