# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#AM_LDFLAGS = \
#-L../src/graphics \-lgraphics \
#-L../src/physics \-lphysics \
#-L../src/input -linput \
#-L../src/common -lcommon \
#-L../src/platform -lplatform \
#-L../src/utils -lutils \
#-L../src/engine -lsmug

AM_CFLAGS = -I../src/

EXTRA_PROGRAMS = app-sprites app-input app-shapes app-linus app-sprites-go app-sprites-go-engine
app_sprites_SOURCES = app-sprites.c
app_input_SOURCES = app-input.c
app_shapes_SOURCES = app-shapes.c
app_sprites_go_SOURCES = app-sprites-go.c
app_sprites_go_engine_SOURCES = app-sprites-go-engine.c
app_linus_SOURCES = app-linus.c
app_linus_LDADD = ../src/libsmug.la
