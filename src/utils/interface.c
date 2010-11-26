#include "interface.h"
#include <smugstd.h>

int smugPrintf(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int return_value = smug_printf(format, arguments);
    va_end(arguments);
    return return_value;
}
