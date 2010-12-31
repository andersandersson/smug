#ifndef SMUGSTD_H_INCLUDED
#define SMUGSTD_H_INCLUDED

#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define smug_printf printf

#ifndef DISABLE_ASSERTS
    void Smug_assert(BOOL result, char* expression, char* file, int line);
    #define smug_assert(x) Smug_assert(x, #x, __FILE__, __LINE__)
#else
    #define smug_assert(x) x
#endif

#endif /* SMUGSTD_H_INCLUDED */
