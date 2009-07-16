#include "log.h"
#include "console.h"

#include <stdarg.h>
#include <stdio.h>

// As default, include all log output
static int gCurrentLogLevel = LOG_DEBUG;


void Log_write(int level, char* prefix, char* file, int line, char* fmt, ...)
{
   char buffer[CONSOLE_PRINT_BUFFER_SIZE];
   char buffer2[CONSOLE_PRINT_BUFFER_SIZE];
   va_list vl;

   va_start(vl, fmt);

   // Only print log if correct log level is set
   if(gCurrentLogLevel <= level) 
      {
         // Print formatted string to buffer 1
         vsprintf(buffer, fmt, vl);

         // Add file and line number to buffer 2
         sprintf(buffer2, "[%s:%d] %s - %s\n", file, line, prefix, buffer);

         // Send buffer 2 to the console
         Console_write(buffer2);
      }

   va_end(vl);
}


void Log_setLevel(int level)
{
   gCurrentLogLevel = level;
}
