/** @file log.h
  * @brief Contains functions for logging engine messages.
  */

#ifndef SMUG_COMMON_LOG_H
#define SMUG_COMMON_LOG_H

#include "common.h"

// Predefined log levels
#define LOG_DEBUG           0x01
#define LOG_WARNING         0x02
#define LOG_ERROR           0x04
#define LOG_NOTIFICATION    0x08
#define LOG_ALL             0xFF

// Define macros for the Log_Write function
#define DEBUG(fmt, ...) Log_addEntry(LOG_DEBUG, "DEBUG", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define WARNING(fmt, ...) Log_addEntry(LOG_WARNING, "WARNING", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) Log_addEntry(LOG_ERROR, "ERROR", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define NOTIFY(fmt, ...) Log_addEntry(LOG_NOTIFICATION, "NOTICE", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Log_print(fmt, ...) _Log_print(LOG_NOTIFICATION, "NOTICE", __FILE__, __LINE__, fmt, 0, ##__VA_ARGS__)
#define Log_printLine(fmt, ...) _Log_print(LOG_NOTIFICATION, "NOTICE", __FILE__, __LINE__, fmt, 1, ##__VA_ARGS__)

/** Initialize the log system (allocate memory, etc)
 */
BOOL Log_init();

/** Terminate the log system (free memory, etc)
 */
void Log_terminate();

/** Write a log entry.
  *
  * Use the macros DEBUG(char*, ...), WARNING(char*, ...), ERROR(char*, ...), etc instead.
  */
void Log_addEntry(int level, char* prefix, char* file, int line, char* fmt, ...);


/** Print log text to console
  * 
  * Use the macro Log_print(char*, ...) instead
  */
void _Log_print(int level, char* prefix, char* file, int line, char* fmt, int newline, ...);


/** Set the log level to be written by Log_Write.
  *
  * @param level A flagset consisting of LOG_DEBUG, LOG_WARNING and/or LOG_ERROR
  */
void Log_setLevel(int level);

/** Set the format string to use for log output
  *
  * The following flags are recognized:
  *
  * %file%     -  The name of the file where the log command was issued
  * %line%     -  The line number in above file
  * %level%    -  The log level string, i.e. DEBUG, NOTICE, WARNING, etc
  * %message%  -  The message printed by the user
  */
void Log_setFormatString(int level, char* format_string);

/** Get the log string set by Log_getFormatString
 */
char* Log_getFormatString(int level);

/** Push a prefix to be prepended to each %message%
 *
 * If you push a prefix to the prefix stack, each following
 * log will be prepended with this until it is poped from the 
 * stack. All prefixes on the stack will be printed.
 */
void Log_pushPrefix(char* prefix);

/** Pop a prefix from the stack
 *
 * Simply removes the last prefix pushed onto the stack
 */
char* Log_popPrefix();

/** Indent the log messages
 * 
 * Make all following log message be indented an extra level
 */
void Log_indent();

/** Dedent the log messages
 * 
 * Make all following log message be indented one level less
 */
void Log_dedent();

/** Set the tab length of the indentation
 * 
 * Set how many spaces one indentation level should have
 */
void Log_setIndentation(unsigned int indent);

#endif // SMUG_COMMON_LOG_H
