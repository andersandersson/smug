#ifndef SMUGSTD_H_INCLUDED
#define SMUGSTD_H_INCLUDED

#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int smug_printf(const char* format, ...);

#ifndef DISABLE_ASSERTS
    void Smug_assert(int result, char* expression, char* file, int line);
    #define smug_assert(x) Smug_assert(x, #x, __FILE__, __LINE__)
#else
    #define smug_assert(x) x
#endif

#endif /* SMUGSTD_H_INCLUDED */
