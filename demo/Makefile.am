# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AM_LDFLAGS = \
-L../src/graphics \-lgraphics \
-L../src/input -linput \
-L../src/common -lcommon \
-L../src/platform -lplatform \
-L../src/utils -lutils 

AM_CFLAGS = -I../src/

EXTRA_PROGRAMS = demo-sprites demo-input
demo_sprites_SOURCES = demo-sprites.c
demo_input_SOURCES = demo-input.c

