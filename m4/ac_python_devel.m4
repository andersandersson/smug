# SYNOPSIS
#
#   AC_PYTHON_DEVEL()
#
# DESCRIPTION
#
#   This macro will check for PYTHON and try to link
#   against it
#

AC_DEFUN([AC_PYTHON_DEVEL],[
	# Required to check which OS we are running
	AC_REQUIRE([AC_CANONICAL_HOST])
	
	# Check the OS used and set Libs corresponding to this
	case $host_os in
	     *cygwin*) python_os=win32
	               PYTHON_LIBS=[-lpython26]
	        ;;	        
	     *linux*)  python_os=x11 
     		       PYTHON_LIBS=[-lpython2.6]
	     	;;
	esac

	# Give option to specify PYTHON directory
	AC_ARG_WITH([python],
		[AS_HELP_STRING([--with-python=PATH], 
		  [The path path containing /lib and /include for PYTHON])], 
		[], 
		[with_python=no]) 

	# If a path has been specified for PYTHON, search this
	# for libs and includes
	if test "$with_python" != "no"; then
	   PYTHON_LDFLAGS=[-L$with_python/libs\ -Xlinker\ -rpath\ $with_python/libs]
	   PYTHON_CFLAGS=[-I$with_python/include]
        else
           PYTHON_LDFLAGS=[$(python-config --ldflags)]
           PYTHON_CFLAGS=[$(python-config --cflags)]
	fi	
    
	# Temporarily save compiler flags
	ac_save_LIBS="$LIBS"
	ac_save_CFLAGS="$CFLAGS"
	ac_save_LDFLAGS="$LDFLAGS"

	# Set compiler flags to work with GLEW	
	LDFLAGS="$LDFLAGS $PYTHON_LDFLAGS"
	LIBS="$LIBS $PYTHON_LIBS"
	CFLAGS="$CFLAGS $PYTHON_CFLAGS"

	# Try to run a simple PYTHON program
	AC_MSG_CHECKING([for PYTHON])
	AC_LANG([C])
	AC_RUN_IFELSE(
	   AC_LANG_PROGRAM([dnl
            [[#include <Python.h>]]
        ],
        [dnl
            [[Py_Initialize();]]
            [[Py_Finalize();]]   
        ]),
	   AC_MSG_RESULT([yes]),
	   AC_MSG_RESULT([no])
	   AC_MSG_ERROR([Could not link with PYTHON]))
       
       
	# Reset compiler flags
	LIBS="$ac_save_LIBS"
	CFLAGS="$ac_save_CFLAGS"
	LDFLAGS="$ac_save_LDFLAGS"
    
])
