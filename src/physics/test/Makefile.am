# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

TESTS = test-vector

AM_CFLAGS = -I../ -I../../

check_PROGRAMS = test-vector
noinst_PROGRAMS = test-polygon

test_vector_SOURCES = test-vector.c
test_vector_LDADD = ../libphysics.la ../../utils/libutils.la ../../common/libcommon.la ../../../test-utils/libtestutils.la
test_polygon_SOURCES = test-polygon.c
test_polygon_LDADD = ../libphysics.la ../../graphics/libgraphics.la ../../utils/libutils.la ../../common/libcommon.la
