# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

SUBDIRS = graphics physics engine

bin_PROGRAMS = smug

smug_SOURCES = main.c

smug_LDADD = engine/libsmug.a
smug_LDADD += graphics/libgraphics.a
smug_LDADD += physics/libphysics.a