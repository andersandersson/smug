# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

TESTS = test-thread

AM_LDFLAGS = -L../ -L../../utils/ -L../../common/ -L../../../test-utils/ -ltestutils -lcommon -lutils -lplatform
AM_CFLAGS = -I../

check_PROGRAMS = test-thread

test_thread_SOURCES = test-thread.c