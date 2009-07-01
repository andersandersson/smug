# Copyright (C) 2009 Anders Andersson <finrist@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## Strip trailing slashes from path 
# 
# This macro will strip any trailing slashes from a path
# or, if the path is empty, set it to '.'
#


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
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

# Guess the hos system (win32, etc)
AC_CANONICAL_BUILD

# Check for C compiler
AC_PROG_CC

# Check for library function
AC_PROG_RANLIB

# Check for Python
AC_PYTHON_DEVEL([>= 2.5])

# Check for GLFW
AC_GLFW_DEVEL([2.6.0])

# Append Python and GLFW flags to default flags
LIBS="$LIBS $GLFW_LIBS $PYTHON_EXTRA_LIBS"
CFLAGS="$CFLAGS $GLFW_CFLAGS $PYTHON_CPPFLAGS"
LDFLAGS="$LDFLAGS $GLFW_LDFLAGS $PYTHON_LDFLAGS $PYTHON_EXTRA_LDFLAGS"

# Check the OS and run corresponding init script
case $build_os in
     linux*) AC_INIT_LINUX() ;;
     *) AC_INIT_WIN32() ;;
esac

# Specify output files
AC_CONFIG_FILES([
   Makefile
   doc/Makefile
   m4/Makefile
   src/Makefile
   src/graphics/Makefile
   src/physics/Makefile
])

AC_OUTPUT
