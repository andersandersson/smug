#include "assert.h"

#include <stdlib.h>
#include <stdio.h>

void SmugTest_assert(int expression, char* assertion, char* file, int line)
{
    if(0 == expression)
        {
            fprintf(stderr, "[%s:%d] - Assertion failed: %s\n", file, line, assertion);
            exit(1);
        }
}
