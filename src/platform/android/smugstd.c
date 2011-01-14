#include <android/log.h>
#include <stdarg.h>

int smug_printf(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int return_value = __android_log_vprint(ANDROID_LOG_INFO, "SMUG", format, arguments);
    va_end(arguments);
    return return_value;
}

#ifndef DISABLE_ASSERTS
void Smug_assert(int result, char* expression, char* file, int line)
{
    if(!result)
    {
        smug_printf("Assertion '%s' failed at line %d in file %s\n", expression, line, file);
    }
}
#endif
