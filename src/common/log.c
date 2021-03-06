#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <smugstd.h>
#include <platform/console.h>
#include <utils/linkedlist.h>
#include <common/log.h>

// As default, include all log output
static int   gCurrentLogLevel = LOG_ALL ^ LOG_NOTIFICATION;
static char* gFormatString = NULL;
static char  gIndentString[1024] = "\0";
static LinkedList* gPrefixStack = NULL;

static int gCurrentlyPrintingLevel = LOG_NONE;

static BOOL _isInitialized(void)
{
    return NULL != gPrefixStack;
}

static void _Console_putsVoid(void* str)
{
    Console_puts((char*)str);
}

static void _writePrefixStack(void)
{
    // Local function: we can assume _isInitialized().
    LinkedList_doList(gPrefixStack, _Console_putsVoid);
}

BOOL Log_init(void)
{
    // Allocate memory for prefix stack
    gPrefixStack = LinkedList_new();

    // Set default format string
    Log_setFormatString(LOG_NOTIFICATION, "[%file%:%line%] - %level% - %message%");

    // Set default indentation level
    Log_setIndentation(4);

    return NULL != gPrefixStack;
}

BOOL Log_isInitialized(void)
{
	return _isInitialized();
}

void Log_terminate(void)
{
    smug_assert(_isInitialized());
    LinkedList_delete(gPrefixStack);
}

void Log_addEntry(int level, char* prefix, char* file, int line, char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    Log_addEntryVa(level, prefix, file, line, fmt, vl);
    va_end(vl);
}

void Log_addEntryVa(int level, char* prefix, char* file, int line, char* fmt, va_list args)
{
    smug_assert(_isInitialized());
    gCurrentlyPrintingLevel = level;

    char* format;
    char message[CONSOLE_PRINT_BUFFER_SIZE];
    char flag[64];
    int  c = 0;
    int  c_max = 0;
    int  flag_c = 0;
    BOOL reading_flag = FALSE;

    // Get the format string for the used log level
    format = Log_getFormatString(level);

    // If no format string is set, don't print anything.
    if(NULL == format) {
        return;
    }

    // Only print log if correct log level is set
    if(gCurrentLogLevel & level)
    {
        // Print formatted string to the message buffer
        vsprintf(message, fmt, args);

        _writePrefixStack();

        // Iterate over each char in the format string and output
        for(c=0,c_max=strlen(format); c<c_max; c++)
        {
            // Start reading a 'flag' when we hit an '%'
            if('%' == format[c] && FALSE == reading_flag)
            {
                reading_flag = TRUE;
                flag_c = 0;
            }
            // Stop reading a 'flag' when we hit an '%' again
            else if('%' == format[c] && TRUE == reading_flag)
            {
                reading_flag = FALSE;

                // %file%
                if(0 == strcmp("file", flag))
                {
                    Console_write(file);
                }

                // %line%
                if(0 == strcmp("line", flag))
                {
                    Console_write("%d", line);
                }

                // %message%
                if(0 == strcmp("message", flag))
                {
                    //_writePrefixStack();
                    Console_write(message);
                }

                // %level%
                if(0 == strcmp("level", flag))
                {
                    Console_write(prefix);
                }
            }
            // If we are currently reading a flag, store
            // it in the flag buffer
            else if(TRUE == reading_flag)
            {
                flag[flag_c] = format[c];
                flag_c++;
                flag[flag_c] = '\0';
            }
            // Otherwise just print the format character
            else
            {
                Console_write("%c", format[c]);
            }
        }

    // Finally, add a newline
    Console_write("%c", '\n');
    }

    gCurrentlyPrintingLevel = LOG_NONE;
}


void _Log_print(int level, char* prefix, char* file, int line, int newline, char* fmt, ...)
{
    gCurrentlyPrintingLevel = level;
    va_list vl;
    char buffer[1024];

    va_start(vl, fmt);

    vsprintf(buffer, fmt, vl);

    Log_addEntry(level, prefix, file, line, buffer);

    _writePrefixStack();
    Console_write(buffer);
    if(1 == newline)
        {
            Console_write("\n");
        }

    va_end(vl);
    gCurrentlyPrintingLevel = LOG_NONE;
}


void Log_setLevel(int level)
{
    smug_assert(_isInitialized());
    gCurrentLogLevel = level;
}

int Log_getLevel(void)
{
    return gCurrentLogLevel;
}

int Log_getCurrentlyPrintingLevel(void)
{
    return gCurrentlyPrintingLevel;
}

void Log_setFormatString(int level, char* format_string)
{
    smug_assert(_isInitialized());
    gFormatString = format_string;
}

char* Log_getFormatString(int level)
{
    smug_assert(_isInitialized());
    return gFormatString;
}

void Log_pushPrefix(char* prefix)
{
    smug_assert(_isInitialized());
    //if(NULL == gPrefixStack)
    //{
    //    gPrefixStack = LinkedList_new();
    //}

    LinkedList_addLast(gPrefixStack, prefix);
}

char* Log_popPrefix(void)
{
    char* item = NULL;
    smug_assert(_isInitialized());

    //if(NULL == gPrefixStack)
    //{
    //    return NULL;
    //}

    if(NULL == gPrefixStack->first)
    {
        return NULL;
    }

    item = gPrefixStack->first->item;

    LinkedList_remove(gPrefixStack, gPrefixStack->first);

    return item;
}

void Log_indent(void)
{
    Log_pushPrefix(gIndentString);
}

void Log_dedent(void)
{
    Log_popPrefix();
}

void Log_setIndentation(unsigned int indent)
{
    unsigned int c = 0;

    for(c=0; c<indent && c<1023; c++)
    {
        gIndentString[c] = ' ';
        gIndentString[c+1] = '\0';
    }
}
