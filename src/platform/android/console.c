#include <android/log.h>
#include <smugstd.h>

#include <common/log.h>
#include <platform/console.h>

static char line_buffer[CONSOLE_PRINT_BUFFER_SIZE + 1];
static int line_buffer_pos = 0;

static android_LogPriority getAndroidLogLevel(int smugLevel)
{
    switch (smugLevel)
    {
        case LOG_DEBUG:
            return ANDROID_LOG_DEBUG;
        case LOG_NOTIFICATION:
            return ANDROID_LOG_INFO;
        case LOG_WARNING:
            return ANDROID_LOG_WARN;
        case LOG_ERROR:
            return ANDROID_LOG_ERROR;
        default:
            return ANDROID_LOG_INFO;
    }
}

void Console_puts(char* str) {
    Console_write("%s", str);
}

static void _reset_line_buffer()
{
    line_buffer_pos = 0;
    memset(line_buffer, '\0', CONSOLE_PRINT_BUFFER_SIZE + 1);
}

static void _compact_line_buffer(int start)
{
    int i1 = 0;
    for (int i2 = start; i2 < line_buffer_pos; i2++)
    {
        line_buffer[i1] = line_buffer[i2];
        i1++;
    }
    line_buffer_pos -= start;
    memset(line_buffer + line_buffer_pos, '\0', CONSOLE_PRINT_BUFFER_SIZE + 1 - line_buffer_pos);
}

void Console_write(char* fmt, ...)
{
    line_buffer[CONSOLE_PRINT_BUFFER_SIZE] = '\0';
    int old_pos = line_buffer_pos;

    va_list vl;
    va_start(vl, fmt);

    // Print formatted string to buffer
    int numchars = vsprintf(line_buffer + line_buffer_pos, fmt, vl);
    line_buffer_pos += numchars;
    if (line_buffer_pos >= CONSOLE_PRINT_BUFFER_SIZE)
    {
        // Overflow. Panic dump buffer.
        line_buffer[CONSOLE_PRINT_BUFFER_SIZE] = '\0';
        __android_log_write(getAndroidLogLevel(Log_getCurrentlyPrintingLevel()), "SMUG", line_buffer);
        _reset_line_buffer();
        return;
    }

    va_end(vl);

    // Iterate over the string and print each character until
    // the end of string is reach, or the buffer size limit.
    int i = old_pos;
    int write_from = 0;
    while(TRUE)
    {
        if (i >= CONSOLE_PRINT_BUFFER_SIZE)
        {
            // Overflow. Panic dump buffer.
            __android_log_write(getAndroidLogLevel(Log_getCurrentlyPrintingLevel()), "SMUG", line_buffer + write_from);
            _reset_line_buffer();
            return;
        }
        if (line_buffer[i] == '\n')
        {
            line_buffer[i] = '\0';
            __android_log_write(getAndroidLogLevel(Log_getCurrentlyPrintingLevel()), "SMUG", line_buffer + write_from);
            write_from = i + 1;
        }
        if (line_buffer[i] == '\0')
        {
            break;
        }
        i++;
    }
    if (write_from > 0)
    {
        _compact_line_buffer(write_from);
    }

    // va_list vl;
    // va_start(vl, fmt);
    // __android_log_vprint(ANDROID_LOG_INFO, "SMUG", fmt, vl);
    // va_end(vl);
}

void Console_writeLine(char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    int numchars = vsprintf(line_buffer + line_buffer_pos, fmt, vl);
    va_end(vl);

    line_buffer_pos += numchars;
    if (line_buffer_pos >= CONSOLE_PRINT_BUFFER_SIZE)
    {
        line_buffer[CONSOLE_PRINT_BUFFER_SIZE] = '\0';
    }
    __android_log_write(getAndroidLogLevel(Log_getCurrentlyPrintingLevel()), "SMUG", line_buffer);
    _reset_line_buffer();
}


unsigned int Console_read(char* dest, unsigned int maxlength)
{
    return 0;
}

void Console_run(void)
{
}

void Console_parseCommand(char* cmd)
{
}

BOOL Console_defaultParser(char* cmd)
{
    return FALSE;
}
