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

###
# Extra parameters for configure script
#
# Provide --with-glfw to set GLFW path
AC_ARG_WITH([glfw], 
		[AS_HELP_STRING([--with-glfw=PATH], 
				[manually set FULL path to glfw installation])], 
		[], 
		[with_glfw=no])

# Provide --enable-win32 to set win32 specific libraries
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

# Set GLFW variables if they are specified
#
# Note: -Xlinker -rath PATH tells the linker to search in PATH
# 	for shared libraries at runtime
#
AS_IF([test "x$with_glfw" != xno],
	    [AS_IF([test "x$with_win32" == xno],
		[AC_SUBST([LIBGLFWLDFLAGS],[-L$with_glfw/lib/x11\ -Xlinker\ -rpath\ $with_glfw/lib/x11])],
		[AC_SUBST([LIBGLFWLDFLAGS],[-L$with_glfw/lib/win32\ -Xlinker\ -rpath\ $with_glfw/lib/win32])])]
	    [AC_SUBST([LIBGLFWCFLAGS],[-I$with_glfw/include])])

###
# Set different libraries depending on if we are in 
# windows or *nix

AS_IF([test "x$with_win32" == xno],
	    # Posix libs
	    [AC_SUBST([LIBGLFW], [-lglfw])]
	    [AC_SUBST([LIBGL], [-lGL])]
	    [AC_SUBST([LIBX], [-lX11])], 
	    # Win32 libs
	    [AC_SUBST([LIBGLFW], [-lglfw])]
	    [AC_SUBST([LIBGL], [-lopengl32])]
	    [AC_SUBST([LIBX], [-lwin32\ -lsystem32])])

###
# Default checks 
#
# Check for C compiler
AC_PROG_CC
# Check for library function
AC_PROG_RANLIB

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
