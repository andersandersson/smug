#ifndef SMUG_COMMON_INTERFACE_H
#define SMUG_COMMON_INTERFACE_H

typedef enum SmugLogLevel
{
    SMUG_LOG_LEVEL_NONE,
    SMUG_LOG_LEVEL_ERROR,
    SMUG_LOG_LEVEL_WARNING,
    SMUG_LOG_LEVEL_NOTIFICATION,
    SMUG_LOG_LEVEL_DEBUG,
    SMUG_LOG_LEVEL_ALL
} SmugLogLevel;

void smugLogSetLevel(SmugLogLevel level);

void smugLogPrint(char* msg);

void smugLog(SmugLogLevel level, char* fmt, ...);

#endif // SMUG_COMMON_INTERFACE_H
