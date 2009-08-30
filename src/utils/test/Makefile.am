# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

TESTS = test-linkedlist test-hook

AM_LDFLAGS = -L../ -L../../common -lutils -lcommon
AM_CFLAGS = -I../ -I../../

check_PROGRAMS = test-linkedlist test-hook

test_linkedlist_SOURCES = test-linkedlist.c
test_hook_SOURCES = test-hook.c