# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

SUBDIRS = graphics physics utils platform

bin_PROGRAMS = smug

smug_SOURCES = main.c \
signal.c signal.h \
common.c common.h \
console.c console.h \
log.c log.h

smug_LDADD = graphics/libgraphics.a
smug_LDADD += physics/libphysics.a
smug_LDADD += platform/libplatform.a
smug_LDADD += utils/libutils.a