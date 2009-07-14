#include "console.h"

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
