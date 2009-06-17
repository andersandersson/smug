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
	AC_ARG_VAR([GLFW_VERSION],[The installed GLWF version to use.])
	
	# Required to check which OS we are running
	AC_REQUIRE([AC_CANONICAL_BUILD])

	AC_MSG_CHECKING([for working installation of GLFW])
	
	# Check the OS used and set Libs corresponding to this
	case $build_os in
	     *-cygwin) glfw_os=win32 ;;
	     *-uwin) glfw_os=win32 ;;
	     *) glfw_os=x11 
     		GLFW_LIBS=[-lglfw\ -lX11\ -lpthread\ -lm\ -lGL\ -lXrandr]
	     	;;
	esac

	# If a path has been specified for GLFW, search this
	# for libs and includes
	if test -n "$GLFW_PATH"; then
	   GLFW_LDFLAGS=-L$GLFW_PATH/lib/$glfw_os
	   GLFW_CFLAGS=-I$GLFW_PATH/include
	fi	

	# Temporarily save compiler flags
	ac_save_LIBS="$LIBS"
	ac_save_CFLAGS="$CFLAGS"
	ac_save_LDFLAGS="$LDFLAGS"

	# Set compiler flags to work with GLFW	
	LDFLAGS=$GLFW_LDFLAGS
	LIBS=$GLFW_LIBS
	CFLAGS=$GLFW_CFLAGS
	
	# Try to link a simple GLFW program
	AC_LANG([C])
	AC_LINK_IFELSE(
	   AC_LANG_PROGRAM([],[dnl
		[[glfwInit();]]
		[[glfwTerminate();]]
	   ]),
	   AC_MSG_RESULT([yes]),
	   AC_MSG_RESULT([no])
	   AC_MSG_ERROR([Could not link with GLFW]))
	
	# Reset compiler flags
	LIBS="$ac_save_LIBS"
	CFLAGS="$ac_save_CFLAGS"
	LDFLAGS="$ac_save_LDFLAGS"
])