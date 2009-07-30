# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Special init commands for Linux
#
AC_DEFUN([AC_INIT_LINUX],
	 [echo "Setting LINUX"])

# Special init commands for Windows
#
# E.g. LIBS=-lsystem32
#
AC_DEFUN([AC_INIT_WIN32],
	 [echo "Setting WIN32"])
	
# Default init parameters
AC_INIT([smug],
        [0.0.1],
        [Anders Andersson finrist@gmail.com],
        [smug])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])

AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([dist-bzip2])

# Guess the hos system (win32, etc)
AC_CANONICAL_HOST

# Check for C compiler
AC_PROG_CC
AM_PROG_CC_C_O

# Check for library function
AC_PROG_LIBTOOL

# Check for Python
AC_PYTHON_DEVEL([>= 2.5])

# Check for GLFW
AC_GLFW_DEVEL([2.6.0])

# Append Python and GLFW flags to default flags
LIBS="$LIBS $GLFW_LIBS $PYTHON_EXTRA_LIBS $PYTHON_EXTRA_LDFLAGS"
CFLAGS="$CFLAGS $GLFW_CFLAGS $PYTHON_CPPFLAGS -Wall"  #-Werror=implicit-function-declaration"
LDFLAGS="$LDFLAGS $GLFW_LDFLAGS $PYTHON_LDFLAGS"

# Check the OS and run corresponding init script
case $host_os in
     linux*) AC_INIT_LINUX() ;;
     *) AC_INIT_WIN32() ;;
esac

# Allow the user to disable use of asserts
AC_ARG_ENABLE([asserts], 
			  [AS_HELP_STRING([--disable-asserts], 
		          [Disable the use of asserts])],
			  [],
			  [enable_asserts=yes])

if test "x$enable_asserts" != "xyes"; then
	AC_DEFINE([DISABLE_ASSERTS], [1], [Disable use of asserts])
fi

# Specify output files
AC_CONFIG_FILES([
   Makefile
   doc/Makefile
   m4/Makefile
   src/Makefile
   src/graphics/Makefile
   src/graphics/test/Makefile 
   src/physics/Makefile 
   src/physics/test/Makefile
   src/engine/Makefile
   src/platform/Makefile
   src/platform/test/Makefile
   src/utils/Makefile
   src/utils/test/Makefile
   src/common/Makefile
   src/common/test/Makefile
   test/Makefile
   api/python/Makefile
   test-utils/Makefile
])

AC_OUTPUT
