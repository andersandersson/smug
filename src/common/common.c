#include "common.h"
#include <smugstd.h>
#include "stdio.h"

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
