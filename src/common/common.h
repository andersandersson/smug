#ifndef SMUG_COMMON_COMMON_H
#define SMUG_COMMON_COMMON_H

#ifdef HAS_CONFIG_H
    #include "config.h"
#endif

typedef unsigned char BOOL;
#define TRUE  1
#define FALSE 0

#undef assert
#ifndef DISABLE_ASSERTS
    void Smug_assert(BOOL result, char* expresison, char* file, int line);
    #define assert(x) Smug_assert(x, #x, __FILE__, __LINE__)
#else
    #define assert(x) x
#endif

typedef double TIME;

#endif // SMUG_COMMON_COMMON_H
