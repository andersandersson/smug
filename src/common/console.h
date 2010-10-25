/** @file console.h
  * @brief Defines a debug console
  * @ingroup smug_common
  */

/** @addtogroup smug_common
  * @{
  */
  
#ifndef SMUG_COMMON_CONSOLE_H
#define SMUG_COMMON_CONSOLE_H

#include "common/common.h" // Using BOOL

/** The maximum size of a line to print to the console
  */
#define CONSOLE_PRINT_BUFFER_SIZE 1024

/** Write simple unformatted text to the console, ending with newline 
 *
 * Makes use of indentation levels.
 * @param str The string to be printed
 */
void Console_puts(char* str);

/** Write text to the console
 *
 * @param fmt printf styled format string.
 */
void Console_write(char* fmt, ...);


/** Write a line of text to the console ending with newline
 *
 * Makes use of indentation levels.
 * @param fmt printf styled format string.
 */
void Console_writeLine(char* fmt, ...);


/** Read input to buffer.
 *
 * Read input to dest and return the number of read
 * bytes. Also, it will not exceed maxlength number
 * of read bytes.
 *
 * @param dest The destination buffer to place input in
 * @param maxlength Do not exceed this length when writing to the buffer
 */
unsigned int Console_read(char* dest, unsigned int maxlength);


/** Start a console loop.
 *
 * This will read input
 * and send to Console_ParseCommand.
 *
 * This function will end on SIG_EXIT.
 */
void Console_run(void);


/** Receive a command and parse it. 
 *
 * This function will call each registred parser with 
 * the commanduntil one of them returns TRUE.
 *
 * @param cmd The command to parse
 */
void Console_parseCommand(char* cmd);


/** Default parser for commands.
 *
 * This function will be called to parse a command if
 * no other function would handle it.
 *
 * @param cmd The command to parse
 * @return TRUE if the command was handled, FALSE otherwise
 */
BOOL Console_defaultParser(char* cmd);


#endif // SMUG_COMMON_CONSOLE_H

/**@}*/
