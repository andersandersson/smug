# SYNOPSIS
#
#   AC_GLFW_DEVEL([version])
#
# DESCRIPTION
#
#   This macro will check for GLFW and try to link
#   against it
#

AC_DEFUN([AC_GLFW_DEVEL],[
	# Required to check which OS we are running
	AC_REQUIRE([AC_CANONICAL_BUILD])
	
	# Check the OS used and set Libs corresponding to this
	case $build_os in
	     *-cygwin) glfw_os=win32 ;;
	     *-uwin) glfw_os=win32 ;;
	     *) glfw_os=x11 
     		GLFW_LIBS=[-lglfw\ -lX11\ -lpthread\ -lm\ -lGL\ -lXrandr]
	     	;;
	esac

	# Give option to specify GLFW directory
	AC_ARG_WITH([glfw],
		[AS_HELP_STRING([--with-glfw=PATH], 
		  [The path path containing /lib and /include for GLFW])], 
		[], 
		[with_glfw=no]) 

	# If a path has been specified for GLFW, search this
	# for libs and includes
	if test "$with_glfw" != "no"; then
	   GLFW_LDFLAGS=[-L$with_glfw/lib/$glfw_os\ -Xlinker\ -rpath\ $with_glfw/lib/$glfw_os]
	   GLFW_CFLAGS=[-I$with_glfw/include]
	fi	

	# Temporarily save compiler flags
	ac_save_LIBS="$LIBS"
	ac_save_CFLAGS="$CFLAGS"
	ac_save_LDFLAGS="$LDFLAGS"

	# Set compiler flags to work with GLFW	
	LDFLAGS=$GLFW_LDFLAGS
	LIBS=$GLFW_LIBS
	CFLAGS=$GLFW_CFLAGS

	# Extract Major, Minor and Revision from version (e.g. 1.2.3 => 1, 2, 3)
	ac_glfw_major=`echo $1 | sed -r 's/^([[0-9]]+)\\..*/\\1/'`
	ac_glfw_minor=`echo $1 | sed -r 's/^[[^.]]*\\.([[0-9]]+)\\..*/\\1/'`
	ac_glfw_revision=`echo $1 | sed -r 's/^[[^.]]*\\.[[^.]]*\\.([[0-9]]+)/\\1/'`

	# Try to run a simple GLFW program
	AC_MSG_CHECKING([for GLFW])
	AC_LANG([C])
	AC_RUN_IFELSE(
	   AC_LANG_PROGRAM([[#include <GL/glfw.h>]],[dnl
		[[glfwInit();]]
		[[glfwTerminate();]]
	   ]),
	   AC_MSG_RESULT([yes]),
	   AC_MSG_RESULT([no])
	   AC_MSG_ERROR([Could not link with GLFW]))
	
	# If we can compile and run, check the version of GLFW
	AC_MSG_CHECKING([for GLFW > $ac_glfw_major.$ac_glfw_minor.$ac_glfw_revision])
	AC_LANG([C])
	AC_RUN_IFELSE(
	   AC_LANG_PROGRAM([[#include <GL/glfw.h>]],[dnl
		[[if(GLFW_VERSION_MAJOR != $ac_glfw_major) return GLFW_VERSION_MAJOR < $ac_glfw_major;]]
		[[if(GLFW_VERSION_MINOR != $ac_glfw_minor) return GLFW_VERSION_MINOR < $ac_glfw_minor;]]
		[[if(GLFW_VERSION_REVISION != $ac_glfw_revision) return GLFW_VERSION_REVISION < $ac_glfw_revision;]]
	   ]),
	   AC_MSG_RESULT([yes]),
	   AC_MSG_RESULT([no])
	   AC_MSG_ERROR([Could not find correct version of GLFW]))


	# Reset compiler flags
	LIBS="$ac_save_LIBS"
	CFLAGS="$ac_save_CFLAGS"
	LDFLAGS="$ac_save_LDFLAGS"
])