/** @file log.h
  * @brief Contains functions for logging engine messages.
  */

#ifndef LOG_H
#define LOG_H

// Predefined log levels
#define LOG_DEBUG    1
#define LOG_WARNING  2
#define LOG_ERROR    3

// Define macros for the Log_Write function
#define DEBUG(x, ...) Log_Write(LOG_DEBUG, "DEBUG", __FILE__, __LINE__, x, __VA_ARGS__)
#define WARNING(x, ...) Log_Write(LOG_WARNING, "WARNING", __FILE__, __LINE__, x, __VA_ARGS__)
#define ERROR(x, ...) Log_Write(LOG_ERROR, "ERROR", __FILE__, __LINE__, x, __VA_ARGS__)

/** Write a log entry.
  *
  * Use the macros DEBUG(char*, ...), WANRING(char*, ...) and ERROR(char*, ...) instead.
  */
void Log_Write(int level, char* prefix, char* file, int line, char* fmt, ...);


/** Set the log level to be written by Log_Write.
  *
  * @param level One of the constants LOG_DEBUG, LOG_WARNING or LOG_ERROR
  */
void Log_SetLevel(int level);

#endif // LOG_H
