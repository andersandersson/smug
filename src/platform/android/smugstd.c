#include <android/log.h>
#include <stdarg.h>
#include <common/common.h>
#include <common/log.h>

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
#ifdef HARD_ASSERTS
        __android_log_assert(expression, "SMUG", "Assertion failure in [%s:%i]", file, line);
#endif
#ifndef HARD_ASSERTS
        // smug_printf("Assertion '%s' failed at line %d in file %s\n", expression, line, file);
        Log_addEntry(LOG_ERROR, "ERROR", file, line, "Assertion failure: '%s'", expression);
#endif
    }
}
#endif
