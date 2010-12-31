#include "common.h"
#include <smugstd.h>
#include "stdio.h"

#ifndef DISABLE_ASSERTS
void Smug_assert(BOOL result, char* expression, char* file, int line)
{
    if(TRUE != result)
    {
        smug_printf("Assertion '%s' failed at line %d in file %s\n", expression, line, file);
        // fprintf(stderr, "Assertion '%s' failed at line %d in file %s\n", expression, line, file);
    }
}
#endif

void swap_float(float* left, float* right)
{
    float temp = *right;
    *right = *left;
    *left = temp;
}

#ifdef SMUG_GLES
long floatToFixed(float value)
{
    if (value < -32768) value = -32768;
    if (value > 32767) value = 32767;
    return (long)(value * 65536);
}
#endif /* SMUG_GLES */
