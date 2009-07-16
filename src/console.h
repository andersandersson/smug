/** @file console.h
  * @brief Defines a debug console
  */

#ifndef CONSOLE_H
#define CONSOLE_H

#include "common.h" // Using BOOL

/** The maximum size of a line to print to the console
 */
#define CONSOLE_PRINT_BUFFER_SIZE 1024


/** Write text to the console
 *
 * @param fmt printf styled format string.
 */
void Console_Write(char* fmt, ...);


/** Read input to buffer.
 *
 * Read input to dest and return the number of read
 * bytes. Also, it will not exceed maxlength number
 * of read bytes.
 *
 * @param dest The destination buffer to place input in
 * @param maxlength Do not exceed this length when writing to the buffer
 */
unsigned int Console_Read(char* dest, unsigned int maxlength);


/** Start a console loop.
 *
 * This will read input
 * and send to Console_ParseCommand.
 *
 * This function will end on SIG_EXIT.
 */
void Console_Run();


/** Receive a command and parse it. 
 *
 * This function will call each registred parser with 
 * the commanduntil one of them returns TRUE.
 *
 * @param cmd The command to parse
 */
void Console_ParseCommand(char* cmd);


/** Default parser for commands.
 *
 * This function will be called to parse a command if
 * no other function would handle it.
 *
 * @param cmd The command to parse
 * @return TRUE if the command was handled, FALSE otherwise
 */
BOOL Console_DefaultParser(char* cmd);

#endif
