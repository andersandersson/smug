#include <smugstd.h>
#include <utils/rectangle.h>
#include "interface.h"

int smugPrintf(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int return_value = smug_printf(format, arguments);
    va_end(arguments);
    return return_value;
}

SmugRect smugRect_Create(float x, float y, float w, float h)
{
    return Rectangle_createFromXYWH(x, y, w, h);
}
