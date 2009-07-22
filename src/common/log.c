#include "log.h"
#include "common/console.h"
#include "utils/linkedlist.h"

#include <stdarg.h>
#include <stdio.h>

// As default, include all log output
static int   gCurrentLogLevel = LOG_ALL;
static char* gFormatString = NULL;
static char  gIndentString[1024] = "\0";
static LinkedList* gPrefixStack = NULL;

static void Log_writePrefixStack()
{
    if(NULL == gPrefixStack)
    {
        return;
    }
    
    LinkedList_doList(gPrefixStack, Console_write);
}

BOOL Log_init()
{
    // Allocate memory for prefix stack
    gPrefixStack = LinkedList_new();
    
    // Set default format string
    Log_setFormatString(LOG_NOTIFICATION, "[%file%:%line%] - %level% - %message%")
    
    // Set default indentation level
    Log_setIndentation(4);
}

void Log_terminate()
{
    LinkedList_delete(gPrefixStack);
}

void Log_write(int level, char* prefix, char* file, int line, char* fmt, ...)
{
   char* format;   
   char message[CONSOLE_PRINT_BUFFER_SIZE];
   char string[CONSOLE_PRINT_BUFFER_SIZE];
   char flag[64];
   int  c = 0;
   int  c_max = 0;
   int  flag_c = 0;
   BOOL reading_flag = FALSE;
   va_list vl;
   
   // Get the format string for the used log level
   format = Log_getFormatString(level);
   
   // If no format string is set, don't print anything.
   if(NULL == format) {
       return;
   }   

   va_start(vl, fmt);

   // Only print log if correct log level is set
   if(gCurrentLogLevel & level) 
      {
         // Print formatted string to the message buffer
         vsprintf(message, fmt, vl);
         
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
                    Log_writePrefixStack();
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

   va_end(vl);
}


void Log_setLevel(int level)
{
   gCurrentLogLevel = level;
}

void Log_setFormatString(int level, char* format_string)
{
    gFormatString = format_string;
}

char* Log_getFormatString(int level)
{
    return gFormatString;
}

void Log_pushPrefix(char* prefix)
{
    if(NULL == gPrefixStack)
    {
        gPrefixStack = LinkedList_new();
    }
    
    LinkedList_addFirst(gPrefixStack, prefix);
}

char* Log_popPrefix()
{
    char* item = NULL;
    
    if(NULL == gPrefixStack)
    {
        return NULL;
    }
 
    if(NULL == gPrefixStack->first)
    {
        return NULL;
    }
    
    item = gPrefixStack->first->item;
    
    LinkedList_remove(gPrefixStack, gPrefixStack->first);
    
    return item;
}

void Log_indent()
{
    Log_pushPrefix(gIndentString);
}

void Log_dedent()
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