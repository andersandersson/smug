# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

TESTS = test-linkedlist test-arraylist test-hook test-vector test-map test-string test-pathname test-orderedset test-map

AM_LDFLAGS = -L../ -L../../common -L../../platform -lutils -lcommon -lplatform
AM_CFLAGS = -I../ -I../../ -I../../common

check_PROGRAMS = test-linkedlist test-arraylist test-hook test-vector test-binarysearchtree test-string test-pathname test-orderedset test-map

test_linkedlist_SOURCES = test-linkedlist.c
test_arraylist_SOURCES = test-arraylist.c
test_hook_SOURCES = test-hook.c
test_vector_SOURCES = test-vector.c
test_binarysearchtree_SOURCES = test-binarysearchtree.c
test_orderedset_SOURCES = test-orderedset.c
test_string_SOURCES = test-string.c
test_pathname_SOURCES = test-pathname.c
test_map_SOURCES = test-map.c
