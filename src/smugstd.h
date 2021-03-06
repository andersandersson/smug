/** This is a dummy file for standard functions used bu smug. Each platform needs to define its own proper versions of these functions (or include their respective standard libraries) in a file with this name, and build with its directory first in the include paths. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include SMUGSTD_H


#ifndef SMUGSTD_H_INCLUDED

#error "This platform has not defined its own version of smugstd.h!"
int smug_printf(const char* format, ...);

#endif //SMUGSTD_H_INCLUDED
