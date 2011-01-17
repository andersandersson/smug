# SYNOPSIS
#
#   AC_GLEW_DEVEL([version])
#
# DESCRIPTION
#
#   This macro will check for GLEW and try to link
#   against it
#

AC_DEFUN([AC_GLEW_DEVEL],[
	# Required to check which OS we are running
	AC_REQUIRE([AC_CANONICAL_HOST])
	
	# Check the OS used and set Libs corresponding to this TODO, Anders fixa linux :)
	case $host_os in
	     *cygwin*) glew_os=win32
	               GLEW_LIBS=[-lglew32s\ -lopengl32]
	        ;;	        
	     *linux*)  glew_os=x11 
     		       GLEW_LIBS=[-lX11\ -lpthread\ -lm\ -lGL\ -lGLEW] #-lXrandr
	     	;;
	esac

	# Give option to specify GLEW directory
	AC_ARG_WITH([glew],
		[AS_HELP_STRING([--with-glew=PATH], 
		  [The path path containing /lib and /include for GLEW])], 
		[], 
		[with_glew=no]) 

	# If a path has been specified for GLEW, search this
	# for libs and includes
	if test "$with_glew" != "no"; then
	   GLEW_LDFLAGS=[-L$with_glew/lib\ -Xlinker\ -rpath\ $with_glew/lib]
	   GLEW_CFLAGS=[-I$with_glew/include\ -DGLEW_STATIC]
	fi	

	# Temporarily save compiler flags
	ac_save_LIBS=$LIBS
	ac_save_CFLAGS=$CFLAGS
	ac_save_LDFLAGS=$LDFLAGS

	# Set compiler flags to work with GLEW	
	LDFLAGS="$LDFLAGS $GLEW_LDFLAGS"
	LIBS="$LIBS $GLEW_LIBS"
	CFLAGS="$CFLAGS $GLEW_CFLAGS"

	# Extract Major, Minor and Revision from version (e.g. 1.2.3 => 1, 2, 3)
	ac_glew_major=`echo $1 | sed -r 's/^([[0-9]]+)\\..*/\\1/'`
	ac_glew_minor=`echo $1 | sed -r 's/^[[^.]]*\\.([[0-9]]+)\\..*/\\1/'`
	ac_glew_revision=`echo $1 | sed -r 's/^[[^.]]*\\.[[^.]]*\\.([[0-9]]+)/\\1/'`

	# Try to run a simple GLEW program
	AC_MSG_CHECKING([for GLEW])
	AC_LANG([C])
	AC_RUN_IFELSE(
	   AC_LANG_PROGRAM([dnl
        [[#include <GL/glew.h>]]
        [[#include <GL/gl.h>]]
        [[#include <GL/glu.h>]]
        ],
        [[glewInit();]]),
	   AC_MSG_RESULT([yes]),
	   AC_MSG_RESULT([no])
	   AC_MSG_ERROR([Could not link with GLEW]))
	
	# If we can compile and run, check the version of GLEW
	AC_MSG_CHECKING([for GLEW > $ac_glew_major.$ac_glew_minor.$ac_glew_revision])
	AC_LANG([C])
	AC_RUN_IFELSE(
	   AC_LANG_PROGRAM([[#include <GL/glew.h>]],[dnl
		[[if(glewGetString(GLEW_VERSION_MAJOR)[0] - '0' != $ac_glew_major) return glewGetString(GLEW_VERSION_MAJOR)[0] - '0' < $ac_glew_major;]]
		[[if(glewGetString(GLEW_VERSION_MINOR)[0] - '0' != $ac_glew_minor) return glewGetString(GLEW_VERSION_MINOR)[0] - '0' < $ac_glew_minor;]]
		[[if(glewGetString(GLEW_VERSION_MICRO)[0] - '0' != $ac_glew_revision) return glewGetString(GLEW_VERSION_MICRO)[0] - '0' < $ac_glew_revision;]]
	   ]),
	   AC_MSG_RESULT([yes]),
	   AC_MSG_RESULT([no])
	   AC_MSG_ERROR([Could not find correct version of GLEW]))


	# Reset compiler flags
	LIBS="$ac_save_LIBS"
	CFLAGS="$ac_save_CFLAGS"
	LDFLAGS="$ac_save_LDFLAGS"
])
