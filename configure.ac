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
AC_DEFUN([AC_STRIP_SLASHES], 
[case ":[$]$1:" in 
      ::) 	   $1='.' ;;
      :*[[\\/]]:)  $1=`echo "[$]$1" | sed 's,[[\\/]]*[$],,'` ;;
      :*:) ;;
esac])

# Default init parameters
AC_INIT([smug],
        [0.0.1],
        [Anders Andersson finrist@gmail.com],
        [smug])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

# Check for C compiler
AC_PROG_CC

# Check for library function
AC_PROG_RANLIB


###
# Extra parameters for configure script
#
# Provide --with-glfw to set GLFW path
AC_ARG_WITH([glfw], 
		[AS_HELP_STRING([--with-glfw=PATH], 
				[manually set FULL path to glfw installation])], 
		[], 
		[with_glfw=no])

# Provie --with-python to set path to python-config
AC_ARG_WITH([python], 
		[AS_HELP_STRING([--with-python=PATH], 
				[manually set FULL path to the python-config binary])], 
		[], 
		[with_python=no])

# Provide --with-win32 to set win32 specific libraries
AC_ARG_WITH([win32], 
		[AS_HELP_STRING([--with-win32], 
				[use this if you are building on windows])], 
		[], 
		[with_win32=no])


###
# Handle extra parameters for configure
#
# Remove trailing slashes and set FLAGS if they are
# specified
AC_STRIP_SLASHES([with_glfw])
AC_STRIP_SLASHES([with_python])

# Set os dependent GLFW path
AS_IF([test "x$with_win32" == xno],
	   [libglfwpath=[$with_glfw/lib/x11]],
	   [libglfwpath=[$with_glfw/lib/win32]])

# Verify that specified glfw and python directories exists
AS_IF([test "x$with_glfw" != xno],
  [AC_CHECK_FILE(
	[$libglfwpath],
	[glfwpathexists=yes],
	[glfwpathexists=no])],
  [])

AS_IF([test "x$with_python" != xno],
  [AC_CHECK_FILE(
	[$with_python],
	[pythonpathexists=yes],
	[pythonpathexists=no])],
  [])

# Set GLFW variables if they are specified
#
# Note: -Xlinker -rath PATH tells the linker to search in PATH
# 	for shared libraries at runtime
#
AS_IF([test "x$with_glfw" != xno],
	    [AS_IF([test "x$with_win32" == xno],
		#[AC_SUBST([LIBGLFWLDFLAGS],[-L$with_glfw/lib/x11\ -Xlinker\ -rpath\ $with_glfw/lib/x11])],
		[libglfwldflags=[-L$libglfwpath\ -Xlinker\ -rpath\ $with_glfw/lib/x11]],
		[libglfwldflags=[-L$libglfwpath\ -Xlinker\ -rpath\ $with_glfw/lib/win32]])]
	    [libglfwcflags=[-I$with_glfw/include]])

###
# Set different libraries depending on if we are in 
# windows or *nix

AS_IF([test "x$with_win32" == xno],
	    # Posix libs
	    [libglfw=[-lglfw]]
	    [libgl=[-lGL]]
	    [ligx=[-lX11]], 
	    # Win32 libs
	    [libglfw=[-lglfw]]
	    [libgl=[-lopengl32]]
	    [libx=[-lwin32\ -lsystem32]])

###
# Default checks 
#

# Check for python-config
AS_IF([test "x$with_python" != xno],
	    [pythonconfig=[$with_python/python-config]],
	    [pythonconfig=[python-config]])

AS_IF([test "$pythonpathexists" = "yes"],
      [AS_IF([test "x$with_python" != xno],
	    [AC_CHECK_PROG([have_python],[python-config],[yes],[no],[$with_python])],
	    [AC_CHECK_PROG([have_python],[python-config],[yes],[no])])],
      [])

# Test to compile a glfw program
#
AS_IF([test "$glfwpathexists" = "yes"],
	    [LDFLAGS=$libglfwldflags]
	    [LIBS=$libglfw],
	    [])

AS_IF([test "$glfwpathexists" = "yes"],
	    [AC_LANG([C])]
	    [AC_COMPILE_IFELSE(
	    	  [AC_LANG_PROGRAM(
			[], 
			[[glfwInit();]]
	  		[[glfwTerminate();]]
	 	  )],
		  [have_glfw=yes],
		  [have_glfw=no]
            )],
	    [])

# Check if Python and Glfw are present, or exit with error otherwise

echo []
echo [Configure results:]

errors=no

AS_IF([test "$glfwpathexists" = "no"],
	    [echo [ \* GLFW path does not exist]]
	    [errors=yes],
	    [])

AS_IF([test "$have_glfw" = "no"],
	    [echo [ \* Could not build GLFW program]]
	    [errors=yes],
	    [])

AS_IF([test "$pythonpathexists" = "no"],
	    [echo [ \* Python path does not exist]]
	    [errors=yes],
	    [])

AS_IF([test "$have_python" = "no"],
	    [echo [ \* Could not run python-config]]
	    [errors=yes],
	    [])

AS_IF([test "$errors" = "yes"],
      [echo []]
      [AC_MSG_ERROR([Exiting due do configure errors])],
      [echo [ - No errors]]
      [echo []])

# Substitute variables in Makefiles
AC_SUBST([LIBGLFWLDFLAGS], [$libglfwldflags])
AC_SUBST([LIBGLFWCFLAGS], [$libglfwcflags])
AC_SUBST([LIBGLFW], [$libglfw])
AC_SUBST([LIBGL], [$libgl])
AC_SUBST([LIBX], [$libx])
AC_SUBST([LIBPYTHONLDFLAGS], [`$pythonconfig --ldflags`])
AC_SUBST([LIBPYTHONCFLAGS], [`$pythonconfig --cflags`])
AC_SUBST([LIBPYTHON], [`$pythonconfig --libs`])

# Specifu output files
AC_CONFIG_FILES([
   Makefile
   README
   doc/Makefile
   m4/Makefile
   src/Makefile
   src/graphics/Makefile
   src/physics/Makefile
])

AC_OUTPUT
