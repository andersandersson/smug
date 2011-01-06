#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <common/signal.h>
#include <common/console.h>

// static unsigned int gIndent = 0;
// static unsigned int gTabWidth = 4;

// Internal function for printing a character
static char _getChar()
{
    return getc(stdin);
}


// Internal function for reading a character
static void _putChar(char c)
{
    putc(c, stderr);
}

void Console_puts(char* str) {
    Console_write("%s", str);
}

void Console_write(char* fmt, ...)
{
    // Counter for writing characters
    int i = 0;

    // Temporary buffer to print formatted string to
    char buffer[CONSOLE_PRINT_BUFFER_SIZE];

    // Variable argument list
    va_list vl;

    // Init variable arguments
    va_start(vl, fmt);

    // Print formatted string to buffer
    vsprintf(buffer, fmt, vl);

    // Iterate over the string and print each character until
    // the end of string is reach, or the buffer size limit.
    while(buffer[i] != '\0' && i < CONSOLE_PRINT_BUFFER_SIZE) {
        _putChar(buffer[i]);
        i++;
    }

    // Finalize variable arguments
    va_end(vl);
}

void Console_writeLine(char* fmt, ...)
{
    // Counter for writing characters
    int i = 0;

    // Temporary buffer to print formatted string to
    char buffer[CONSOLE_PRINT_BUFFER_SIZE];

    // Variable argument list
    va_list vl;

    // Init variable arguments
    va_start(vl, fmt);

    // Print formatted string to buffer
    vsprintf(buffer, fmt, vl);

    // Iterate over the string and print each character until
    // the end of string is reach, or the buffer size limit.
    while(buffer[i] != '\0' && i < CONSOLE_PRINT_BUFFER_SIZE) {
        _putChar(buffer[i]);
        i++;
    }

    // End line
    _putChar('\n');

    // Finalize variable arguments
    va_end(vl);
}


unsigned int Console_read(char* dest, unsigned int maxlength)
{
    int i = 0;
    char c = 0;

    // Read characters until newline or maxlength is reached
    while(i < maxlength-1 && c != '\n')
    {
        c = _getChar();
        dest[i] = c;
        i++;
    }

    // Replace the newline with a string terminator
    dest[i-1] = '\0';

    return i;
}

void Console_run(void)
{
    char buffer[1024];

    while(!Signal_check(SIG_TERM))
    {
        // Read next command
        Console_read(buffer, 1024);

        // Parse next command
        Console_parseCommand(buffer);
    }
}

void Console_parseCommand(char* cmd)
{
    // If no parser would handle the command, print
    // an error message
    if(TRUE != Console_defaultParser(cmd))
    {
        Console_write("Unknown command: %s\n", cmd);
    }
}

BOOL Console_defaultParser(char* cmd)
{
    // exit - Send exit signal
    if(0 == strcmp("exit", cmd))
    {
        Signal_send(SIG_EXIT);
    }
    return TRUE;
}
