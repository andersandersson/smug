/** @file log.h
  * @brief Contains functions for logging engine messages.
  */

#ifndef LOG_H
#define LOG_H

// Predefined log levels
#define LOG_NOTIFICATION   0
#define LOG_DEBUG   1
#define LOG_WARNING 2
#define LOG_ERROR   4
#define LOG_ALL  	7

// Define macros for the Log_Write function
#define DEBUG(fmt, ...) Log_write(LOG_DEBUG, "DEBUG", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define WARNING(fmt, ...) Log_write(LOG_WARNING, "WARNING", __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) Log_write(LOG_ERROR, "ERROR", __FILE__, __LINE__, fmt, ##__VA_ARGS__)

/** Write a log entry.
  *
  * Use the macros DEBUG(char*, ...), WANRING(char*, ...) and ERROR(char*, ...) instead.
  */
void Log_write(int level, char* prefix, char* file, int line, char* fmt, ...);


/** Set the log level to be written by Log_Write.
  *
  * @param level A flagset consisting of LOG_DEBUG, LOG_WARNING and/or LOG_ERROR
  */
void Log_setLevel(int level);

#endif // LOG_H
