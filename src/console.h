#ifndef CONSOLE_H
#define CONSOLE_H

// The maximum size of a line to print to the console
#define CONSOLE_PRINT_BUFFER_SIZE 1024

// Write text to the console
void Console_Write(char* fmt, ...);

// Read input to dest and return the number of read
// bytes. Also, it will not exceed maxlength number
// of read bytes.
unsigned int Console_Read(char* dest, unsigned int maxlength);

#endif
