#include "common.h"

#include "stdio.h"

#ifndef DISABLE_ASSERTS
void Smug_assert(BOOL result, char* expression, char* file, int line)
{
    if(TRUE != result)
    {
        fprintf(stderr, "Assertion '%s' failed at line %d in file %s\n", expression, line, file);
    }
}
#endif

void swap_float(float* left, float* right)
{
    float temp = *right;
    *right = *left;
    *left = temp;
}
