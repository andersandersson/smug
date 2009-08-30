# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

TESTS = test-image test-renderer

AM_LDFLAGS = -L../ -lgraphics -L../../common -lcommon -L../../utils -L../../platform -lutils -lplatform
AM_CFLAGS = -I../ -I../../ -I../../common

check_PROGRAMS = test-image test-renderer

test_image_SOURCES = test-image.c

test_renderer_SOURCES = test-renderer.c
