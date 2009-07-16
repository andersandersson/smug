#include "console.h"

#include "signal.h"

#include <stdio.h>
#include <stdarg.h>


// Internal function for printing a character
static char Console_GetChar()
{
   return getc(stdin);
}


// Internal function for reading a character
static void Console_PutChar(char c)
{
   putc(c, stderr);
}


void Console_Write(char* fmt, ...)
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
      Console_PutChar(buffer[i]);
      i++;
   }

   // Finalize variable arguments
   va_end(vl);
}


unsigned int Console_Read(char* dest, unsigned int maxlength)   
{
   int i = 0;
   char c = 0;
   
   // Read characters until newline or maxlength is reached
   while(i < maxlength-1 && c != '\n')
      {
         c = Console_GetChar();
         dest[i] = c;
         i++;
      }
   
   // Replace the newline with a string terminator
   dest[i-1] = '\0';

   return i;
}


void Console_Run()
{
   char buffer[1024];

   while(!Signal_Check(SIG_EXIT))
      {
         // Print console prompt
         Console_Write("> ");

         // Read next command
         Console_Read(buffer, 1024);

         // Parse next command
         Console_ParseCommand(buffer);
      }
}


void Console_ParseCommand(char* cmd)
{
   // If no parser would handle the command, print
   // an error message
   if(TRUE != Console_DefaultParser(cmd))
      {
         Console_Write("Unknown command: %s\n", cmd);
      }
}


BOOL Console_DefaultParser(char* cmd)
{
   // exit - Send exit signal
   if(0 == strcmp("exit", cmd)) {
      Signal_Send(SIG_EXIT);
   }
}
