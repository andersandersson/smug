#ifdef HAS_CONFIG_H
    #include "config.h"
#endif

typedef unsigned char BOOL;
#define TRUE  1
#define FALSE 0
#define NULL  0

#ifndef DISABLE_ASSERTS
    void smug_assert(BOOL result, line)
    {
        if(TRUE != result)
        {
            fprintf(stderr, "Assert: Failed at line %d", line);
        }
    }

    #define assert(x) smug_assert(x, __LINE)
#else
    #define assert(x)
#endif