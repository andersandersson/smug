#ifndef COMMON_H
#define COMMON_H

#ifdef HAS_CONFIG_H
    #include "config.h"
#endif

typedef unsigned char BOOL;
#define TRUE  1
#define FALSE 0

#ifndef DISABLE_ASSERTS
    void Smug_assert(BOOL result, int line);
    #define assert(x) Smug_assert(x, __LINE)
#else
    #define assert(x) x
#endif

typedef double Time;

#endif // COMMON_H

