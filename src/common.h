#ifndef COMMON_H
#define COMMON_H

#ifdef HAS_CONFIG_H
    #include "config.h"
#endif

typedef unsigned char BOOL;
#define TRUE  1
#define FALSE 0
#define NULL  0

#ifndef DISABLE_ASSERTS
    #define assert(x) smug_assert(x, __LINE)
#else
    #define assert(x)
#endif

#endif // COMMON_h
