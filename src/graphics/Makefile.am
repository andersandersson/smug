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

AM_CFLAGS = $(LIBGLFWCFLAGS)  -I..

noinst_LTLIBRARIES = libgraphics.la

libgraphics_la_SOURCES =  interface.c interface.h graphics.c graphics.h \
drawable.c drawable.h \
color.c color.h \
sprite.c sprite.h \
image/image.c image/image.h \
image/lodepng/lodepng.c image/lodepng/lodepng.h
