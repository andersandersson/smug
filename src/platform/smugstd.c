#include <platform/smugstd.h>

#ifndef DISABLE_ASSERTS
void Smug_assert(BOOL result, char* expression, char* file, int line)
{
    if(!result)
    {
        smug_printf("Assertion '%s' failed at line %d in file %s\n", expression, line, file);
    }
}
#endif
